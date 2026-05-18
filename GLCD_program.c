/*
 * GLCD_program.c
 *
 * Created: 5/18/2026 8:45:11 PM
 *  Author: hamza
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "STD_types.h"
#include "Bit_Math.h"
#include "DIO_interface.h"
#include "GLCD_config.h"
#include "GLCD_interface.h"
#include "GLCD_private.h"

/* -----------------------------------------------------------------------
 * 5x7 ASCII font table (printable chars 0x20 – 0x7E)
 * Each character = 5 bytes, one byte per column, MSB = top pixel
 * ----------------------------------------------------------------------- */
static const u8 Font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, /* ' ' */
    {0x00,0x00,0x5F,0x00,0x00}, /* '!' */
    {0x00,0x07,0x00,0x07,0x00}, /* '"' */
    {0x14,0x7F,0x14,0x7F,0x14}, /* '#' */
    {0x24,0x2A,0x7F,0x2A,0x12}, /* '$' */
    {0x23,0x13,0x08,0x64,0x62}, /* '%' */
    {0x36,0x49,0x55,0x22,0x50}, /* '&' */
    {0x00,0x05,0x03,0x00,0x00}, /* ''' */
    {0x00,0x1C,0x22,0x41,0x00}, /* '(' */
    {0x00,0x41,0x22,0x1C,0x00}, /* ')' */
    {0x14,0x08,0x3E,0x08,0x14}, /* '*' */
    {0x08,0x08,0x3E,0x08,0x08}, /* '+' */
    {0x00,0x50,0x30,0x00,0x00}, /* ',' */
    {0x08,0x08,0x08,0x08,0x08}, /* '-' */
    {0x00,0x60,0x60,0x00,0x00}, /* '.' */
    {0x20,0x10,0x08,0x04,0x02}, /* '/' */
    {0x3E,0x51,0x49,0x45,0x3E}, /* '0' */
    {0x00,0x42,0x7F,0x40,0x00}, /* '1' */
    {0x42,0x61,0x51,0x49,0x46}, /* '2' */
    {0x21,0x41,0x45,0x4B,0x31}, /* '3' */
    {0x18,0x14,0x12,0x7F,0x10}, /* '4' */
    {0x27,0x45,0x45,0x45,0x39}, /* '5' */
    {0x3C,0x4A,0x49,0x49,0x30}, /* '6' */
    {0x01,0x71,0x09,0x05,0x03}, /* '7' */
    {0x36,0x49,0x49,0x49,0x36}, /* '8' */
    {0x06,0x49,0x49,0x29,0x1E}, /* '9' */
    {0x00,0x36,0x36,0x00,0x00}, /* ':' */
    {0x00,0x56,0x36,0x00,0x00}, /* ';' */
    {0x08,0x14,0x22,0x41,0x00}, /* '<' */
    {0x14,0x14,0x14,0x14,0x14}, /* '=' */
    {0x00,0x41,0x22,0x14,0x08}, /* '>' */
    {0x02,0x01,0x51,0x09,0x06}, /* '?' */
    {0x32,0x49,0x79,0x41,0x3E}, /* '@' */
    {0x7E,0x11,0x11,0x11,0x7E}, /* 'A' */
    {0x7F,0x49,0x49,0x49,0x36}, /* 'B' */
    {0x3E,0x41,0x41,0x41,0x22}, /* 'C' */
    {0x7F,0x41,0x41,0x22,0x1C}, /* 'D' */
    {0x7F,0x49,0x49,0x49,0x41}, /* 'E' */
    {0x7F,0x09,0x09,0x09,0x01}, /* 'F' */
    {0x3E,0x41,0x49,0x49,0x7A}, /* 'G' */
    {0x7F,0x08,0x08,0x08,0x7F}, /* 'H' */
    {0x00,0x41,0x7F,0x41,0x00}, /* 'I' */
    {0x20,0x40,0x41,0x3F,0x01}, /* 'J' */
    {0x7F,0x08,0x14,0x22,0x41}, /* 'K' */
    {0x7F,0x40,0x40,0x40,0x40}, /* 'L' */
    {0x7F,0x02,0x0C,0x02,0x7F}, /* 'M' */
    {0x7F,0x04,0x08,0x10,0x7F}, /* 'N' */
    {0x3E,0x41,0x41,0x41,0x3E}, /* 'O' */
    {0x7F,0x09,0x09,0x09,0x06}, /* 'P' */
    {0x3E,0x41,0x51,0x21,0x5E}, /* 'Q' */
    {0x7F,0x09,0x19,0x29,0x46}, /* 'R' */
    {0x46,0x49,0x49,0x49,0x31}, /* 'S' */
    {0x01,0x01,0x7F,0x01,0x01}, /* 'T' */
    {0x3F,0x40,0x40,0x40,0x3F}, /* 'U' */
    {0x1F,0x20,0x40,0x20,0x1F}, /* 'V' */
    {0x3F,0x40,0x38,0x40,0x3F}, /* 'W' */
    {0x63,0x14,0x08,0x14,0x63}, /* 'X' */
    {0x07,0x08,0x70,0x08,0x07}, /* 'Y' */
    {0x61,0x51,0x49,0x45,0x43}, /* 'Z' */
    {0x00,0x7F,0x41,0x41,0x00}, /* '[' */
    {0x02,0x04,0x08,0x10,0x20}, /* '\' */
    {0x00,0x41,0x41,0x7F,0x00}, /* ']' */
    {0x04,0x02,0x01,0x02,0x04}, /* '^' */
    {0x40,0x40,0x40,0x40,0x40}, /* '_' */
    {0x00,0x01,0x02,0x04,0x00}, /* '`' */
    {0x20,0x54,0x54,0x54,0x78}, /* 'a' */
    {0x7F,0x48,0x44,0x44,0x38}, /* 'b' */
    {0x38,0x44,0x44,0x44,0x20}, /* 'c' */
    {0x38,0x44,0x44,0x48,0x7F}, /* 'd' */
    {0x38,0x54,0x54,0x54,0x18}, /* 'e' */
    {0x08,0x7E,0x09,0x01,0x02}, /* 'f' */
    {0x0C,0x52,0x52,0x52,0x3E}, /* 'g' */
    {0x7F,0x08,0x04,0x04,0x78}, /* 'h' */
    {0x00,0x44,0x7D,0x40,0x00}, /* 'i' */
    {0x20,0x40,0x44,0x3D,0x00}, /* 'j' */
    {0x7F,0x10,0x28,0x44,0x00}, /* 'k' */
    {0x00,0x41,0x7F,0x40,0x00}, /* 'l' */
    {0x7C,0x04,0x18,0x04,0x78}, /* 'm' */
    {0x7C,0x08,0x04,0x04,0x78}, /* 'n' */
    {0x38,0x44,0x44,0x44,0x38}, /* 'o' */
    {0x7C,0x14,0x14,0x14,0x08}, /* 'p' */
    {0x08,0x14,0x14,0x18,0x7C}, /* 'q' */
    {0x7C,0x08,0x04,0x04,0x08}, /* 'r' */
    {0x48,0x54,0x54,0x54,0x20}, /* 's' */
    {0x04,0x3F,0x44,0x40,0x20}, /* 't' */
    {0x3C,0x40,0x40,0x20,0x7C}, /* 'u' */
    {0x1C,0x20,0x40,0x20,0x1C}, /* 'v' */
    {0x3C,0x40,0x30,0x40,0x3C}, /* 'w' */
    {0x44,0x28,0x10,0x28,0x44}, /* 'x' */
    {0x0C,0x50,0x50,0x50,0x3C}, /* 'y' */
    {0x44,0x64,0x54,0x4C,0x44}, /* 'z' */
    {0x00,0x08,0x36,0x41,0x00}, /* '{' */
    {0x00,0x00,0x7F,0x00,0x00}, /* '|' */
    {0x00,0x41,0x36,0x08,0x00}, /* '}' */
    {0x10,0x08,0x08,0x10,0x08}, /* '~' */
};

/* Page shadow buffer (128x64 = 128 bytes x 8 pages) */
static u8 g_au8Buffer[8][128];

/* -----------------------------------------------------------------------
 * Private helpers
 * ----------------------------------------------------------------------- */

static void PRV_voidSelectChip(u8 copy_u8Chip)
{
    if (copy_u8Chip == GLCD_LEFT_CHIP) {
        DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, DIO_PIN_HIGH);
        DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, DIO_PIN_LOW);
    } else {
        DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, DIO_PIN_LOW);
        DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, DIO_PIN_HIGH);
    }
}

static void PRV_voidEnablePulse(void)
{
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, DIO_PIN_HIGH);
    _delay_us(1);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, DIO_PIN_LOW);
    _delay_us(1);
}

static void PRV_voidSendCommand(u8 copy_u8Chip, u8 copy_u8Cmd)
{
    PRV_voidSelectChip(copy_u8Chip);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RS_PIN, DIO_PIN_LOW);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RW_PIN, DIO_PIN_LOW);
    DIO_voidSetPortValue(GLCD_DATA_PORT, copy_u8Cmd);
    PRV_voidEnablePulse();
}

static void PRV_voidSendData(u8 copy_u8Chip, u8 copy_u8Data)
{
    PRV_voidSelectChip(copy_u8Chip);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RS_PIN, DIO_PIN_HIGH);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RW_PIN, DIO_PIN_LOW);
    DIO_voidSetPortValue(GLCD_DATA_PORT, copy_u8Data);
    PRV_voidEnablePulse();
}

/* Flush one page column from buffer to hardware */
static void PRV_voidFlushPixel(u8 copy_u8Page, u8 copy_u8Col)
{
    u8 chip = (copy_u8Col < GLCD_HALF_WIDTH) ? GLCD_LEFT_CHIP : GLCD_RIGHT_CHIP;
    u8 col  = (copy_u8Col < GLCD_HALF_WIDTH) ? copy_u8Col : (copy_u8Col - GLCD_HALF_WIDTH);

    PRV_voidSendCommand(chip, GLCD_CMD_SET_PAGE | copy_u8Page);
    PRV_voidSendCommand(chip, GLCD_CMD_SET_Y    | col);
    PRV_voidSendData  (chip, g_au8Buffer[copy_u8Page][copy_u8Col]);
}

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

void GLCD_voidInit(void)
{
    /* Direction setup */
    DIO_voidSetPortDirection(GLCD_DATA_PORT, DIO_PORT_OUTPUT);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_RS_PIN,  DIO_PIN_OUTPUT);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_RW_PIN,  DIO_PIN_OUTPUT);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_EN_PIN,  DIO_PIN_OUTPUT);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_CS1_PIN, DIO_PIN_OUTPUT);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_CS2_PIN, DIO_PIN_OUTPUT);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_RST_PIN, DIO_PIN_OUTPUT);

    /* Hardware reset */
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RST_PIN, DIO_PIN_LOW);
    _delay_ms(10);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RST_PIN, DIO_PIN_HIGH);
    _delay_ms(10);

    PRV_voidSendCommand(GLCD_LEFT_CHIP,  GLCD_CMD_DISPLAY_ON);
    PRV_voidSendCommand(GLCD_RIGHT_CHIP, GLCD_CMD_DISPLAY_ON);
    PRV_voidSendCommand(GLCD_LEFT_CHIP,  GLCD_CMD_SET_START | 0);
    PRV_voidSendCommand(GLCD_RIGHT_CHIP, GLCD_CMD_SET_START | 0);

    GLCD_voidClear();
}

void GLCD_voidClear(void)
{
    u8 page, col;
    for (page = 0; page < 8; page++) {
        for (col = 0; col < GLCD_WIDTH; col++) {
            g_au8Buffer[page][col] = 0x00;
            PRV_voidFlushPixel(page, col);
        }
    }
}

void GLCD_voidSetPixel(u8 copy_u8X, u8 copy_u8Y, u8 copy_u8Color)
{
    if (copy_u8X >= GLCD_WIDTH || copy_u8Y >= GLCD_HEIGHT) return;

    u8 page = copy_u8Y / 8;
    u8 bit  = copy_u8Y % 8;

    if (copy_u8Color) {
        SET_BIT(g_au8Buffer[page][copy_u8X], bit);
    } else {
        CLR_BIT(g_au8Buffer[page][copy_u8X], bit);
    }
    PRV_voidFlushPixel(page, copy_u8X);
}

void GLCD_voidGoTo(u8 copy_u8Page, u8 copy_u8Col)
{
    /* used externally for reference; actual positioning done per-char */
    (void)copy_u8Page;
    (void)copy_u8Col;
}

void GLCD_voidDisplayChar(u8 copy_u8Page, u8 copy_u8Col, char copy_cChar)
{
    u8 i;
    if (copy_cChar < 0x20 || copy_cChar > 0x7E) copy_cChar = ' ';

    const u8* fontCol = Font5x7[(u8)(copy_cChar - 0x20)];

    for (i = 0; i < 5; i++) {
        u8 col = copy_u8Col + i;
        if (col >= GLCD_WIDTH) break;
        g_au8Buffer[copy_u8Page][col] = fontCol[i];
        PRV_voidFlushPixel(copy_u8Page, col);
    }
    /* 1-pixel gap */
    if ((copy_u8Col + 5) < GLCD_WIDTH) {
        g_au8Buffer[copy_u8Page][copy_u8Col + 5] = 0x00;
        PRV_voidFlushPixel(copy_u8Page, copy_u8Col + 5);
    }
}

void GLCD_voidDisplayString(u8 copy_u8Page, u8 copy_u8Col, const char* copy_pcStr)
{
    while (*copy_pcStr) {
        GLCD_voidDisplayChar(copy_u8Page, copy_u8Col, *copy_pcStr);
        copy_u8Col += 6;
        copy_pcStr++;
        if (copy_u8Col >= GLCD_WIDTH) break;
    }
}

/* -----------------------------------------------------------------------
 * Waveform drawing
 * Pages 2–7 are used (y pixel 16–63 ? pages 2–7)
 * The waveform occupies a 128-wide, 32-pixel tall area (pages 2–5)
 * ----------------------------------------------------------------------- */
void GLCD_voidDrawWaveform(f32 copy_f32DutyCycle)
{
    u8 x, page;
    u8 highPixels;  /* number of columns in HIGH state */

    /* Clear waveform area (pages 2–5) */
    for (page = 2; page <= 5; page++) {
        for (x = 0; x < GLCD_WIDTH; x++) {
            g_au8Buffer[page][x] = 0x00;
            PRV_voidFlushPixel(page, x);
        }
    }

    if (copy_f32DutyCycle < 0.0f)   copy_f32DutyCycle = 0.0f;
    if (copy_f32DutyCycle > 100.0f) copy_f32DutyCycle = 100.0f;

    highPixels = (u8)((copy_f32DutyCycle * (f32)GLCD_WIDTH) / 100.0f);

    /* Waveform area: pages 2–5 = y 16–47  (32 pixels tall)
     * HIGH signal  ? top half    pages 2–3 (y 16–31)
     * LOW  signal  ? bottom half pages 4–5 (y 32–47)
     * Transitions are vertical lines.
     */

    /* Draw HIGH portion on pages 2–3 */
    for (x = 0; x < highPixels && x < GLCD_WIDTH; x++) {
        /* Full byte = all 8 bits on in page */
        g_au8Buffer[2][x] = 0xFF;
        g_au8Buffer[3][x] = 0xFF;
        PRV_voidFlushPixel(2, x);
        PRV_voidFlushPixel(3, x);
    }

    /* Draw LOW portion on pages 4–5 */
    for (x = highPixels; x < GLCD_WIDTH; x++) {
        g_au8Buffer[4][x] = 0xFF;
        g_au8Buffer[5][x] = 0xFF;
        PRV_voidFlushPixel(4, x);
        PRV_voidFlushPixel(5, x);
    }

    /* Rising edge (left edge of HIGH): vertical line at x=0, pages 2-5 */
    if (highPixels > 0) {
        g_au8Buffer[2][0] = 0xFF;
        g_au8Buffer[3][0] = 0xFF;
        g_au8Buffer[4][0] = 0xFF;
        g_au8Buffer[5][0] = 0xFF;
        PRV_voidFlushPixel(2, 0);
        PRV_voidFlushPixel(3, 0);
        PRV_voidFlushPixel(4, 0);
        PRV_voidFlushPixel(5, 0);
    }

    /* Falling edge: vertical line at x = highPixels */
    if (highPixels > 0 && highPixels < GLCD_WIDTH) {
        x = highPixels;
        g_au8Buffer[2][x] = 0xFF;
        g_au8Buffer[3][x] = 0xFF;
        g_au8Buffer[4][x] = 0xFF;
        g_au8Buffer[5][x] = 0xFF;
        PRV_voidFlushPixel(2, x);
        PRV_voidFlushPixel(3, x);
        PRV_voidFlushPixel(4, x);
        PRV_voidFlushPixel(5, x);
    }

    /* Baseline (bottom of LOW area): page 5, bit 7 ? all columns */
    for (x = 0; x < GLCD_WIDTH; x++) {
        SET_BIT(g_au8Buffer[5][x], 7);
        PRV_voidFlushPixel(5, x);
    }
    /* Top line of HIGH area: page 2, bit 0 ? columns in HIGH */
    for (x = 0; x < highPixels; x++) {
        SET_BIT(g_au8Buffer[2][x], 0);
        PRV_voidFlushPixel(2, x);
    }
}

/* -----------------------------------------------------------------------
 * Info bar on pages 0–1
 * Layout:
 *   Page 0 col 0  : "F:XX.XkHz"
 *   Page 0 col 72 : "D:XX%"
 *   Page 1 col 0  : "T:XXXXX us"
 * ----------------------------------------------------------------------- */
void GLCD_voidDisplayInfo(f32 copy_f32FreqKHz, f32 copy_f32DutyCycle, u32 copy_u32PeriodUs)
{
    char buf[22];
    u8 page, col;

    for (page = 0; page <= 1; page++) {
        for (col = 0; col < GLCD_WIDTH; col++) {
            g_au8Buffer[page][col] = 0x00;
            PRV_voidFlushPixel(page, col);
        }
    }

    /* Row 0 left : "Freq:XX.XXkHz" */
    snprintf(buf, sizeof(buf), "Freq:%.2fkHz", (double)copy_f32FreqKHz);
    GLCD_voidDisplayString(0, 0, buf);

    /* Row 0 right: "D:XX.X%"  starts at col 84 */
    snprintf(buf, sizeof(buf), "D:%.1f%%", (double)copy_f32DutyCycle);
    GLCD_voidDisplayString(0, 84, buf);

    /* Row 1 left : "T:XXXXXus" */
    snprintf(buf, sizeof(buf), "T:%luus", (unsigned long)copy_u32PeriodUs);
    GLCD_voidDisplayString(1, 0, buf);
}
