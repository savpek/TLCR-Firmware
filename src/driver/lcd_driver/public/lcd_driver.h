/*
 * lcd_driver.h
 *
 * Created: 21.2.2012 23:01:01
 *  Author: savpek
 */ 
#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include "compiler.h"

/*! \brief This struct contains pin configuration of display.
 *	\struct lcd_drv_t */
typedef const struct {
	/* TODO: Pin definitions */
} lcd_drv_t;

/*!	\brief Init lcd display driver, set correct pins and IO settings for them ...
 *		   lcd display also requires certain INIT routines before use. 
 *	
 *	@param[in] lcd_settings : Struct contains all settings for current lcd display. */
extern void lcd_driver_init( void);

/*!	\brief Put character to LCD display. Replaces character if there is any. 
 *
 *	@param[in] lcd_settings : Struct that contains configuration for display. 
 *	@param[in] pox_x : Character position in line, first index of line is 0.
 *	@param[in] pos_y : Line from display, first line index 0, second 1 ... 
 *  @param[in] put_this_char : 
 *	@return VOID */
extern void lcd_driver_put_char( lcd_drv_t *lcd_settings, uint8_t pos_x, uint8_t pos_y, char put_this_char);

/*! \brief Clear lcd display. 
 *
 *	@param None. 
 *	@return VOID */
extern void lcd_driver_clear_display( void);

/*! \brief Set lcd display on!
 *
 *	@param None. 
 *	@return VOID*/
extern void lcd_driver_display_on( void);

/*! \brief Set lcd display off! 
 *
 *  @param None.
 *	@return Void. */
extern void lcd_driver_display_off( void);




#endif /* LCD_DRIVER_H_ */