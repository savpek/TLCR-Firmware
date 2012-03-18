/*
 * lcd_driver_config.h
 *
 * Created: 23.2.2012 0:27:52
 *  Author: savpek
 */


#ifndef LCD_DRIVER_CONFIG_H_
#define LCD_DRIVER_CONFIG_H_

#include "./lcd_driver/private/lcd_driver_private.h"

#define LCD_D_CLEAR_DISPLAY (1<<LCD_D_DB0)
#define LCD_D_RET_HOME (1<<LCD_D_DB1)

/*! Define here initial settings for entry mode.
 *	(1<<LCD_D_DB2) : Entry mode command flag.
 *	(1<<LCD_D_DB1) : Change cursor moving direction at display.
 *	(1<<LCD_D_DB0) : To enable shift of entire display.*/
#define LCD_D_ENTRY_INIT (1<<LCD_D_DB2)

/*! Define on/off to displays.
 *	(1<<LCD_D_DB3) : On/off command flag.
 *	(1<<LCD_D_DB2) : Set this to put display ON.
 *	(1<<LCD_D_DB1) : Set cursor ON.
 *	(1<<LCD_D_DB0) : Set cursor blink. */
#define LCD_D_SET_ON (1<<LCD_D_DB3) | (1<<LCD_D_DB2)
#define LCD_D_SET_OFF (1<<LCD_D_DB3)

/*! Define initial settings for cursor and display shift.
 *	(1<<LCD_D_DB4) : Shift command flag.
 *	(1<<LCD_D_DB3) : Set this if you want that cursor shifts.
 *	(1<<LCD_D_DB2) : Set this if you want that cursor (and display) shifts from right to left. */
#define LCD_D_SHIFT_INIT (1<<LCD_D_DB4)

/*! Define initial settings for function set.
 *	(1<<LCD_D_DB5) : Function set command flag.
 *	(1<<LCD_D_DB4) : Set interface to 4 BIT. (0: 8bit)
 *	(1<<LCD_D_DB3) : Set interface to 1 LINE. (0: 2 lines)
 *	(1<<LCD_D_DB3) : Set display font to 5x8 dots. (0: 5x11 dots) */
#define LCD_D_FUNCTION_SET_INIT (1<<LCD_D_DB5) | (1<<LCD_D_DB4)

/*! Command to write data to ram.
 *	Usage: LCD_D_SEND_DATA_MASK & DATA_TO_WRITE .*/
#define LCD_D_SEND_CHAR_MASK (1<<LCD_D_RS)

/*! Command to set addres in RAM.
 *	Usage: LCD_D_SEND_ADDR_MASK | (ADDRES_TO_WRITE_NEXT_TIME & LCD_D_SEND_DDRAM_ADDR_MASK). */
#define LCD_D_SEND_DDRAM_ADDR_CMD (1<<LCD_DB6)
#define LCD_D_SEND_DDRAM_ADDR_MASK (0x3F) /* Accecpt 6 addr bits, ignore rest. */

/*! Command to set addres in GRAM.
 *	Usage: LCD_D_SEND_GRAM_ADDR_MASK & (ADDRES_TO_WRITE_NEXT_TIME & LCD_D_SEND_CGRAM_ADDR_MASK).*/
#define LCD_D_SEND_CGRAM_ADDR_CMD (1<<LCD_DB7)
#define LCD_D_SEND_CGRAM_ADDR_MASK (0x7F) /* Accept 7 addr bits, ignore rest. */

#endif /* LCD_DRIVER_CONFIG_H_ */