#ifndef PWM_INTERFACE_H_
#define PWM_INTERFACE_H_

#include "STD_types.h"

void  PWM_DRAWER_voidInit(void);
void  PWM_DRAWER_voidGeneratePWM(u16 copy_u16FrequencyHz, f32 copy_f32DutyCycle);
void  PWM_DRAWER_voidStopPWM(void);

u32   PWM_DRAWER_u32GetPeriod(void);
u32   PWM_DRAWER_u32GetOnTime(void);
f32   PWM_DRAWER_f32GetDutyCycle(void);
f32   PWM_DRAWER_f32GetFrequency(void);

#endif
