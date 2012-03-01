/*!
 *	@file .\api\ioapi\test\test_ioapi.c
 *  @author: savpek
 *
 *	@brief Test file for unity.
 */

#include "./utest/public/utest.h"
#include "./gpio/public/gpio.h"
#include "./ioapi/public/ioapi.h"

/*! @brief Set-up test group for: str
 *	@param Group name */
TEST_GROUP(ioapi);

/*! @brief Group setup function..
 *	@param Group name */
TEST_SETUP(ioapi) {

}

/*! @brief Group teardown function..
 *	@param Group name */
TEST_TEAR_DOWN(ioapi) {

}

/*! @brief Test ioapi_output_high
 *	@param Group name
 *	@param Test name */
TEST(ioapi, test_ioapi_output_high) {
	gpio_configure_pin(2, GPIO_DIR_OUTPUT);
	ioapi_output_high(2);
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_GPIO_USED );
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_OUTPUT);
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_OUT_HIGH);
}

/*! @brief Test ioapi_output_low
 *	@param Group name
 *	@param Test name */
TEST(ioapi, test_ioapi_output_low) {
	gpio_configure_pin(2, GPIO_DIR_OUTPUT);
	ioapi_output_low(2);
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_GPIO_USED );
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_OUTPUT);
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_OUT_LOW);
}

/*! @brief Test ioapi_input_value
 *	@param Group name
 *	@param Test name */
TEST(ioapi, test_ioapi_input_value) {
	/* Cannot truly test this functionality, because register values
	 * depends HW. */
	gpio_configure_pin(2, GPIO_DIR_INPUT);
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_GPIO_USED );
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_INPUT);
}

/*! @brief Test get pin info functionality.
 *	@param Group name
 *	@param Test name */
TEST(ioapi, test_ioapi_get_pin_info) {
	gpio_configure_pin(2, GPIO_DIR_OUTPUT);
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_GPIO_USED );
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_OUTPUT );

	gpio_configure_pin(2, GPIO_DIR_INPUT);
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_GPIO_USED );
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_INPUT );
	
	gpio_configure_pin(2, GPIO_DIR_INPUT|GPIO_PULL_UP );
	TEST_ASSERT(ioapi_get_pin_info(2) & IOAPI_PULL_UP );
}

/*	@brief Set up all runnable tests from this module.
 *	@param group name.*/
TEST_GROUP_RUNNER(ioapi) {
	RUN_TEST_CASE(ioapi, test_ioapi_get_pin_info);
	RUN_TEST_CASE(ioapi, test_ioapi_output_high);
	RUN_TEST_CASE(ioapi, test_ioapi_output_low);
	RUN_TEST_CASE(ioapi, test_ioapi_input_value);
}