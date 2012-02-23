/*
 * lcd_driver_private.h
 *
 * Created: 22.2.2012 9:38:29
 *  Author: savpek
 */ 


#ifndef LCD_DRIVER_PRIVATE_H_
#define LCD_DRIVER_PRIVATE_H_

enum lcd_bit_pattern_t {
	LCD_D_DB0 = 0,
	LCD_D_DB1 = 1,
	LCD_D_DB2 = 2,
	LCD_D_DB3 = 3,
	LCD_D_DB4 = 4,
	LCD_D_DB5 = 5,
	LCD_D_DB6 = 6,
	LCD_D_DB7 = 7,
	LCD_D_RW = 8,
	LCD_D_RS = 9
};

/*! \brief */
extern void (*lcd_driver_write_cmd)(uint32_t cmd);

#endif /* LCD_DRIVER_PRIVATE_H_ */