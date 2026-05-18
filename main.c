/*
 * PWM_test.c
 *
 * Created: 2/28/2026 12:52:36 AM
 * Author : hamza
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "GLCD_interface.h"
#include "PWM_DRAWER_interface.h"
#include "PWM_DRAWER_config.h"

int main(void)
{
	GLCD_voidInit();
	PWM_DRAWER_voidInit();

	while (1)
	{
		f32 freq_hz  = PWM_DRAWER_f32GetFrequency();
		f32 duty     = PWM_DRAWER_f32GetDutyCycle();
		u32 period_u = (PWM_DRAWER_u32GetPeriod() == 0)
		? 0
		: (u32)((f32)PWM_DRAWER_u32GetPeriod()
		* ((f32)PWM_DRAWER_ICU_PRESCALER / (f32)PWM_DRAWER_F_CPU)
		* 1000000.0f);

		GLCD_voidDisplayInfo((f32)freq_hz / 1000.0f, duty, period_u);
		GLCD_voidDrawWaveform(duty);

		_delay_ms(50);
	}
}
