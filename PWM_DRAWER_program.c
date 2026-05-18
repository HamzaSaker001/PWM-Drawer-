#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "DIO_interface.h"
#include "PWM_DRAWER_interface.h"
#include "PWM_DRAWER_config.h"

/*
 * Timer split:
 *   Timer0  -> Fast PWM on OC0 (PB3) for internal test signal generation.
 *   Timer1  -> Normal mode, Input Capture Unit (ICP1 = PD6) for measurement.
 *
 * Three-edge capture sequence:
 *   Edge 1 (rising)  -> capture C1, switch to falling
 *   Edge 2 (falling) -> capture C2, switch to rising
 *   Edge 3 (rising)  -> capture C3
 *   OnTime  = C2 - C1
 *   Period  = C3 - C1
 *   16-bit subtraction wraps correctly on overflow.
 */

static volatile u16 g_u16C1     = 0;
static volatile u16 g_u16C2     = 0;
static volatile u8  g_u8State   = 0;
static volatile u32 g_u32Period = 0;
static volatile u32 g_u32OnTime = 0;

void PWM_DRAWER_voidInit(void)
{
    /* --- Timer1: Normal mode, prescaler = PWM_DRAWER_ICU_PRESCALER (8) --- */
    TCCR1A_REG = 0x00;
    TCCR1B_REG = (1 << ICES1) | (1 << CS11);   /* rising edge first, prescaler 8 */
    TCCR1A_REG = 0x00;
    TCNT1  = 0;

    TIMSK |= (1 << TICIE1);                 /* enable input capture interrupt  */

    g_u8State   = 0;
    g_u32Period = 0;
    g_u32OnTime = 0;

#if PWM_DRAWER_USE_INTERNAL_SRC
    PWM_DRAWER_voidGeneratePWM(PWM_DRAWER_INTERNAL_FREQ_HZ, PWM_DRAWER_INTERNAL_DUTY);
#endif

    sei();
}

void PWM_DRAWER_voidGeneratePWM(u16 copy_u16FrequencyHz, f32 copy_f32DutyCycle)
{
    if (copy_f32DutyCycle > 100.0f || copy_u16FrequencyHz == 0) return;

    DIO_voidSetPinDirection(PWM_DRAWER_PWM_PORT, PWM_DRAWER_PWM_PIN, DIO_PIN_OUTPUT);

   

    u32 target = (u32)copy_u16FrequencyHz;
    u8  cs     = 0;
    u32 actual = 0;

    struct { u8 bits; u32 div; } prescalers[] = {
        {(1<<CS00),                     1   },
        {(1<<CS01),                     8   },
        {(1<<CS01)|(1<<CS00),           64  },
        {(1<<CS02),                     256 },
        {(1<<CS02)|(1<<CS00),           1024}
    };

    u32 bestErr = 0xFFFFFFFF;
    for (u8 i = 0; i < 5; i++) {
        u32 f = PWM_DRAWER_F_CPU / (prescalers[i].div * 256UL);
        u32 err = (f > target) ? (f - target) : (target - f);
        if (err < bestErr) { bestErr = err; cs = prescalers[i].bits; actual = f; }
    }
    (void)actual;

    /* OCR0 sets duty: OCR0 = (duty/100) * 255 */
    u8 ocr0 = (u8)((copy_f32DutyCycle * 255.0f) / 100.0f);

    TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | cs;
    OCR0  = ocr0;
}

void PWM_DRAWER_voidStopPWM(void)
{
    TCCR0 = 0x00;
    OCR0  = 0x00;
    DIO_voidSetPinValue(PWM_DRAWER_PWM_PORT, PWM_DRAWER_PWM_PIN, DIO_PIN_LOW);
}

ISR(TIMER1_CAPT_vect)
{
    u16 capture = ICR1;

    if (g_u8State == 0)
    {
        g_u16C1   = capture;
        g_u8State = 1;
        CLR_BIT(TCCR1B, ICES1);            /* switch to falling edge */
    }
    else if (g_u8State == 1)
    {
        g_u16C2   = capture;
        g_u8State = 2;
        SET_BIT(TCCR1B, ICES1);            /* switch to rising edge  */
    }
    else
    {
        g_u32OnTime = (u32)(g_u16C2 - g_u16C1);    /* high time: falling - rising  */
        g_u32Period = (u32)(capture  - g_u16C1);    /* period:  2nd rising - 1st    */
        g_u8State   = 0;
        SET_BIT(TCCR1B, ICES1);                     /* arm for next rising edge     */
    }
}

u32 PWM_DRAWER_u32GetPeriod(void)  { return g_u32Period; }
u32 PWM_DRAWER_u32GetOnTime(void)  { return g_u32OnTime; }

f32 PWM_DRAWER_f32GetDutyCycle(void)
{
    if (g_u32Period == 0) return 0.0f;
    return ((f32)g_u32OnTime / (f32)g_u32Period) * 100.0f;
}

f32 PWM_DRAWER_f32GetFrequency(void)
{
    if (g_u32Period == 0) return 0.0f;
    return ((f32)PWM_DRAWER_F_CPU / (f32)PWM_DRAWER_ICU_PRESCALER) / (f32)g_u32Period;
}
