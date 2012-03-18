/*
 * test_spy_lcd_driver.c
 *
 * Created: 22.2.2012 11:47:15
 *  Author: savpek
 */

#include "compiler.h"
#include "./utest/public/utest.h"
#include "./lcd_driver/spy/spy_lcd_driver.h"
#include "./lcd_driver/private/lcd_driver_private.h"

uint8_t cmd_buffer_idx = 0;
volatile uint32_t cmd_input_buffer[30] = {~0};

void spy_lcd_driver_write_cmd(uint32_t cmd_input) {
	if (cmd_buffer_idx > 30-1) TEST_FAIL_MESSAGE("...send_cmd, out_of_bounds");
	cmd_input_buffer[cmd_buffer_idx] = cmd_input;
	cmd_buffer_idx++;
}
void (*backup_lcd_driver_write_cmd)(uint32_t) = NULL;

void spy_lcd_driver_cmd_buffer_reset() {
	int i = 0;
	for ( i = 0; i < 30-1; i++) cmd_input_buffer[i] = ~0;
	cmd_buffer_idx = 0;
}

uint32_t spy_read_lcd_driver_cmd(uint8_t idx) {
	if (idx > cmd_buffer_idx) TEST_FAIL_MESSAGE("...buffer_get, out_of_bounds");
	return cmd_input_buffer[idx];
}