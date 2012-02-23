/*
 * IncFile1.h
 *
 * Created: 22.2.2012 11:55:57
 *  Author: savpek
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "compiler.h"

extern void spy_lcd_driver_send_cmd(uint32_t cmd_input);
void spy_lcd_driver_cmd_buffer_reset(void);
uint32_t spy_read_lcd_driver_send_cmd(uint8_t idx);
#endif /* INCFILE1_H_ */