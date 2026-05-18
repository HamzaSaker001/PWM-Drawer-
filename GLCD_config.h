#ifndef GLCD_CONFIG_H_
#define GLCD_CONFIG_H_

#define GLCD_DATA_PORT     DIO_PORTC

#define GLCD_CTRL_PORT     DIO_PORTB

#define GLCD_RS_PIN        DIO_PIN0
#define GLCD_RW_PIN        DIO_PIN1
#define GLCD_EN_PIN        DIO_PIN2
#define GLCD_CS1_PIN       DIO_PIN6   /* moved from PIN3 to avoid OC0 conflict */
#define GLCD_CS2_PIN       DIO_PIN4
#define GLCD_RST_PIN       DIO_PIN5

#endif
