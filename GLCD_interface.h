/*
 * GLCD_interface.h
 *
 * Created: 5/18/2026 8:45:52 PM
 *  Author: hamza
 */ 


#ifndef GLCD_INTERFACE_H_
#define GLCD_INTERFACE_H_

#include "STD_types.h"

/* GLCD physical dimensions */
#define GLCD_WIDTH         128
#define GLCD_HEIGHT        64
#define GLCD_HALF_WIDTH    64

/* Chip select selectors */
#define GLCD_LEFT_CHIP     0
#define GLCD_RIGHT_CHIP    1

void GLCD_voidInit(void);
void GLCD_voidClear(void);

/* Low-level pixel */
void GLCD_voidSetPixel(u8 copy_u8X, u8 copy_u8Y, u8 copy_u8Color);

/* Text rendering (5x7 font) */
void GLCD_voidGoTo(u8 copy_u8Page, u8 copy_u8Col);
void GLCD_voidDisplayChar(u8 copy_u8Page, u8 copy_u8Col, char copy_cChar);
void GLCD_voidDisplayString(u8 copy_u8Page, u8 copy_u8Col, const char* copy_pcStr);

/* Waveform drawing */
void GLCD_voidDrawWaveform(f32 copy_f32DutyCycle);

/* Info bar: freq, duty, period */
void GLCD_voidDisplayInfo(f32 copy_f32FreqKHz, f32 copy_f32DutyCycle, u32 copy_u32PeriodUs);

#endif /* GLCD_INTERFACE_H_ */