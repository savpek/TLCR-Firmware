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

/*! @brief Set-up test group for: str
 *	@param Group name */
TEST_GROUP(test_lcd_private);

/*! @brief Group setup function..
 *	@param Group name */
TEST_SETUP(test_lcd_private) {

}

/*! @brief Group teardown function..
 *	@param Group name */
TEST_TEAR_DOWN(test_lcd_private) {

}
/*! @brief Test that routine sends commands correctly to ioapi.
 *	@param Group name
 *	@param Test name */
TEST(test_lcd_private, test_lcd_driver_write_cmd) {
	lcd_driver_write_cmd(0x00);

	/* When writing command to LCD, there should be following sequence:
	 * Change DATA to correct.
	 * Change RS state.
	 * Set RW state.
	 * Set E high.
	 * Wait for moment.
	 * Set E low. Done.
	 *
	 * Also should be noticed that data is given to send CMD in 8 bit's at time,
	 * but display is used at 4 bits interface. So every time write routines are done
	 * twice during single command. First send high 4 bits, then low.
	 */
	TEST_ASSERT( spy_ioapi_tracker[0].cmd | IOAPI_OUT_HIGH );
	//TEST_ASSERT( spy_ioapi_tracker[0].pin |  );
}
/*! @brief TEMPLATE.
 *	@param Group name
 *	@param Test name */
TEST(test_lcd_private, test_simple_io) {

}

/*	@brief Set up all runnable tests from this module. 
 *	@param group name.*/
TEST_GROUP_RUNNER(test_lcd_private) {
	//RUN_TEST_CASE(test_lcd_cmd_write, test_simple_io);
}