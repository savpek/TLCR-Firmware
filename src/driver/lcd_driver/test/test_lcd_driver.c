/*!	@brief Test LCD driver.
 *	@file test_lcd_driver.c
 *
 *  @author: savpek
 */

#include "./utest/public/utest.h"
#include "./lcd_driver/spy/spy_lcd_driver.h"
#include "./lcd_driver/public/lcd_driver.h"
#include "./lcd_driver/private/lcd_driver_private.h"
#include "./lcd_driver/config/lcd_driver_config.h"

/*! @brief Set-up test group for: lcd_driver()
 *	@param Group name */
TEST_GROUP(lcd_driver);

/*! @brief Group setup function..
 *	@param Group name */
TEST_SETUP(lcd_driver) {
	/* We need lcd_driver spy: */
	if(backup_lcd_driver_write_cmd == NULL)
		backup_lcd_driver_write_cmd = lcd_driver_write_cmd;
	lcd_driver_write_cmd = spy_lcd_driver_write_cmd;
}

/*! @brief Group teardown function..
 *	@param Group name */
TEST_TEAR_DOWN(lcd_driver) {
	lcd_driver_write_cmd = backup_lcd_driver_write_cmd;
	spy_lcd_driver_cmd_buffer_reset();
}

/*! @brief Test that init set up routines correctly.
 *	@param Group name
 *	@param Test name */
TEST(lcd_driver, init) {
	lcd_driver_init();

	/* Init 4 bit interface */
	TEST_ASSERT_EQUAL_HEX32((1<<LCD_D_DB5)|(1<<LCD_D_DB4)|(1<<LCD_D_DB1)|(1<<LCD_D_DB0), spy_read_lcd_driver_cmd(0));
	TEST_ASSERT_EQUAL_HEX32((1<<LCD_D_DB5)|(1<<LCD_D_DB4)|(1<<LCD_D_DB1), spy_read_lcd_driver_cmd(1));

	TEST_ASSERT_EQUAL_HEX32(LCD_D_FUNCTION_SET_INIT, spy_read_lcd_driver_cmd(2));
	TEST_ASSERT_EQUAL_HEX32(LCD_D_SET_OFF, spy_read_lcd_driver_cmd(3));
	TEST_ASSERT_EQUAL_HEX32(LCD_D_CLEAR_DISPLAY, spy_read_lcd_driver_cmd(4));
	TEST_ASSERT_EQUAL_HEX32(LCD_D_ENTRY_INIT, spy_read_lcd_driver_cmd(5));
	TEST_ASSERT_EQUAL_HEX32(LCD_D_SET_ON, spy_read_lcd_driver_cmd(6));
}

/*! @brief Test lcd off functionality.
 *	@param Group name
 *	@param Test name */
TEST(lcd_driver, set_lcd_off) {
	lcd_driver_display_off();
	TEST_ASSERT_EQUAL_HEX32(LCD_D_SET_OFF, spy_read_lcd_driver_cmd(0));
}

/*! @brief Test lcd on functionlity..
 *	@param Group name
 *	@param Test name */
TEST(lcd_driver, set_lcd_on) {
	lcd_driver_display_on();
	TEST_ASSERT_EQUAL_HEX32(LCD_D_SET_ON, spy_read_lcd_driver_cmd(0));
}

/* @brief Set up all runnable test from this module. */
TEST_GROUP_RUNNER(lcd_driver) {
	RUN_TEST_CASE(lcd_driver, init);
	RUN_TEST_CASE(lcd_driver, set_lcd_off);
	RUN_TEST_CASE(lcd_driver, set_lcd_on);
}