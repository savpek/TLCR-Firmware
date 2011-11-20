/*
 * lcd_ctrl_functions.h
 *
 * Created: 8.9.2011 15:22:26
 *  Author: savpek
 */ 


#ifndef LCD_CTRL_FUNCTIONS_H_
#define LCD_CTRL_FUNCTIONS_H_

#include "lcd.h"

#define LCD_CMD_CLR_DISPLAY
#define LCD_CMD

/* Low level data transmission routines */
extern void lcd_e_routine(void);
 
extern void lcd_send_cmd(uint8_t command);
extern void lcd_send_char(uint8_t char_to_send);

/* This function waits until */ 
extern void lcd_wait_busy_end(void);

/* This inits display. MCU io for lcd etc... */
extern void lcd_init_display(void);

/* This rewrites display information corresponding to lcd_handle 
 * variable. lcd_handle is defined in lcd_config.h/c files.*/
extern void lcd_rewrite_display(lcd_t display_params);

#endif /* LCD_CTRL_FUNCTIONS_H_ */