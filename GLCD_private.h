/*
 * GLCD_private.h
 *
 * Created: 5/18/2026 8:46:36 PM
 *  Author: hamza
 */ 


#ifndef GLCD_PRIVATE_H_
#define GLCD_PRIVATE_H_

/* KS0108 command set */
#define GLCD_CMD_DISPLAY_ON    0x3F
#define GLCD_CMD_DISPLAY_OFF   0x3E
#define GLCD_CMD_SET_Y         0x40   /* | col (0-63) */
#define GLCD_CMD_SET_PAGE      0xB8   /* | page (0-7) */
#define GLCD_CMD_SET_START     0xC0   /* | line (0-63) */

/* Internal helpers */
static void PRV_voidSendCommand(u8 copy_u8Chip, u8 copy_u8Cmd);
static void PRV_voidSendData(u8 copy_u8Chip, u8 copy_u8Data);
static void PRV_voidSelectChip(u8 copy_u8Chip);
static void PRV_voidEnablePulse(void);

#endif /* GLCD_PRIVATE_H_ */