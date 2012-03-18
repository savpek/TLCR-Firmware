/*!
 *	@file D:\Dropbox\AVR32\TLCR\FIRMWARE\src\driver\lcd_driver\test\test_lcd_cmd_write.c
 *  @author: savpek
 *
 *	@brief Test file for unity.
 */

#include "./utest/public/utest.h"
#include "./ioapi/public/ioapi.h"
#include "./ioapi/spy/spy_ioapi.h"
#include "./lcd_driver/private/lcd_driver_private.h"
#include "./lcd_driver/config/lcd_driver_config.h"

/*! @brief Set-up test group for: str
 *	@param Group name */
TEST_GROUP(lcd_driver_private);

/*! @brief Group setup function..
 *	@param Group name */
TEST_SETUP(lcd_driver_private) {
	if(backup_ioapi_output_high == NULL)
		backup_ioapi_output_high = ioapi_output_high;
	ioapi_output_high = spy_ioapi_output_high;

	if(backup_ioapi_output_low == NULL)
		backup_ioapi_output_low = ioapi_output_low;
	ioapi_output_low = spy_ioapi_output_low;
	
	/* Resets io history */
	spy_ioapi_reset_tracker();
}

/*! @brief Group teardown function..
 *	@param Group name */
TEST_TEAR_DOWN(lcd_driver_private) {
	ioapi_output_high = backup_ioapi_output_high;

	ioapi_output_low = backup_ioapi_output_low;
}

#define CHECK_PIN(pin_number, cmd_check, order_number) \
	TEST_ASSERT_EQUAL_HEX32_MESSAGE( cmd_check, (spy_ioapi_tracker[order_number]).cmd&cmd_check, "Invalid cmd!" );\
	TEST_ASSERT_EQUAL_UINT8_MESSAGE( pin_number ,(spy_ioapi_tracker[order_number]).pin, "Invalid pin!" )

/*! @brief Test that routine sends commands correctly to ioapi.
 *	@param Group name
 *	@param Test name */
TEST(lcd_driver_private, lcd_driver_write_cmd_addr_0x00) {
	lcd_driver_write_cmd(0x00);

	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_LOW, 0);

	/* Set RS, in command set addr to 0x00 it is low! */
	CHECK_PIN(IOPIN_LCD_RS, IOAPI_OUT_LOW, 1);

	/* Set databits, first write high ones in 4 bit mode. */
	CHECK_PIN(IOPIN_LCD_DB4, IOAPI_OUT_LOW, 2);
	CHECK_PIN(IOPIN_LCD_DB5, IOAPI_OUT_LOW, 3);
	CHECK_PIN(IOPIN_LCD_DB6, IOAPI_OUT_LOW, 4);
	CHECK_PIN(IOPIN_LCD_DB7, IOAPI_OUT_LOW, 5);

	/* RW is pulled down allways in at hardware! */
	
	/* E-line-routines */
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_HIGH, 6);
	
	/* Here should be brief pause, defined in display datasheet.
	 * However delay is too complex to test accurately so passed ... */
	
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_LOW, 7);
	
	/* Set databits (again), lower ones now. */
	CHECK_PIN(IOPIN_LCD_DB4, IOAPI_OUT_LOW, 8);
	CHECK_PIN(IOPIN_LCD_DB5, IOAPI_OUT_LOW, 9);
	CHECK_PIN(IOPIN_LCD_DB6, IOAPI_OUT_LOW, 10);
	CHECK_PIN(IOPIN_LCD_DB7, IOAPI_OUT_LOW, 11);

	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_HIGH, 12);
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_LOW, 13);
}
/*! @brief Test command with value 0xFF.
 *	@param Group name
 *	@param Test name */
TEST(lcd_driver_private, lcd_driver_write_cmd_addr_0xFF) {
	lcd_driver_write_cmd(0xFF);
	
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_LOW, 0);
	CHECK_PIN(IOPIN_LCD_RS, IOAPI_OUT_LOW, 1);
	CHECK_PIN(IOPIN_LCD_DB4, IOAPI_OUT_HIGH, 2);
	CHECK_PIN(IOPIN_LCD_DB5, IOAPI_OUT_HIGH, 3);
	CHECK_PIN(IOPIN_LCD_DB6, IOAPI_OUT_HIGH, 4);
	CHECK_PIN(IOPIN_LCD_DB7, IOAPI_OUT_HIGH, 5);
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_HIGH, 6);
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_LOW, 7);

	CHECK_PIN(IOPIN_LCD_DB4, IOAPI_OUT_HIGH, 8);
	CHECK_PIN(IOPIN_LCD_DB5, IOAPI_OUT_HIGH, 9);
	CHECK_PIN(IOPIN_LCD_DB6, IOAPI_OUT_HIGH, 10);
	CHECK_PIN(IOPIN_LCD_DB7, IOAPI_OUT_HIGH, 11);
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_HIGH, 12);
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_LOW, 13);
}

/*! @brief Test command with character 'a'.
 *	@param Group name
 *	@param Test name */
TEST(lcd_driver_private, lcd_driver_write_cmd_character_a) {
	lcd_driver_write_cmd(LCD_D_SEND_CHAR_MASK|'a');
	
	CHECK_PIN(IOPIN_LCD_E, IOAPI_OUT_LOW, 0);
	CHECK_PIN(IOPIN_LCD_RS, IOAPI_OUT_HIGH, 1);
	CHECK_PIN(IOPIN_LCD_DB4, IOAPI_OUT_LOW, 2);
	CHECK_PIN(IOPIN_LCD_DB5, IOAPI_OUT_HIGH, 3);
	CHECK_PIN(IOPIN_LCD_DB6, IOAPI_OUT_HIGH, 4);
	CHECK_PIN(IOPIN_LCD_DB7, IOAPI_OUT_LOW, 5);
	
}

/*	@brief Set up all runnable tests from this module.
 *	@param group name.*/
TEST_GROUP_RUNNER(lcd_driver_private) {
	RUN_TEST_CASE(lcd_driver_private, lcd_driver_write_cmd_addr_0x00);
	RUN_TEST_CASE(lcd_driver_private, lcd_driver_write_cmd_addr_0xFF);
	RUN_TEST_CASE(lcd_driver_private, lcd_driver_write_cmd_character_a);
}