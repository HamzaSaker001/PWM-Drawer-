#ifndef PWM_CONFIG_H_
#define PWM_CONFIG_H_

#define PWM_DRAWER_F_CPU            16000000UL
#define PWM_DRAWER_ICU_PRESCALER    8UL


#define PWM_DRAWER_PWM_PORT         DIO_PORTB
#define PWM_DRAWER_PWM_PIN          DIO_PIN3

#define PWM_DRAWER_USE_INTERNAL_SRC  1
#define PWM_DRAWER_INTERNAL_FREQ_HZ  1000U
#define PWM_DRAWER_INTERNAL_DUTY     40.0f

#define TCCR1A_REG  (*(volatile u8*)0x4F)
#define TCCR1B_REG  (*(volatile u8*)0x4E)

#endif
