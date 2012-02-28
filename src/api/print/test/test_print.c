/*!
 *	@file D:\Dropbox\AVR32\TLCR\FIRMWARE\src\api\print\test\test_print.c
 *  @author: savpek
 *
 *	@brief Test file for unity.
 */

#include "./utest/public/utest.h"
#include "./print/public/print.h"
#include "./usart/public/usart.h"
#include "./usart/spy/spy_usart.h"

#include "./print/public/print.h"

void (*putchar_storage)(char) = NULL;

/*! You must change output streams during tests because you need same
 *	output driver for unity error messages! */
#define SET_DEFAULT_OUTPUT() usart_putchar = putchar_storage;
#define SET_SPY_OUTPUT()	 usart_putchar = spy_usart_putchar;

/*! @brief Set-up test group for: str
 *	@param Group name */
TEST_GROUP(print);

/*! @brief Group setup function..
 *	@param Group name */
TEST_SETUP(print) {
	putchar_storage = usart_putchar;
	usart_putchar = spy_usart_putchar;
	spy_usart_reset_tx();
}

/*! @brief Group teardown function..
 *	@param Group name */
TEST_TEAR_DOWN(print) {
	usart_putchar =	putchar_storage;
}

/*! @brief Test print_char function from api.
 *	@param Group name
 *	@param Test name */
TEST(print, test_print_char) {
	spy_usart_reset_tx();

	SET_SPY_OUTPUT();
	print_char('b');
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_HEX8('b', spy_usart_buffer_tx[0]);

	SET_SPY_OUTPUT();
	print_char('c');
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_HEX8('c', spy_usart_buffer_tx[1]);
}

/*! @brief Test print_string
 *	@param Group name
 *	@param Test name */
TEST(print, test_print_line) {
	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_line("this is test line");
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("this is test line\n\r", spy_usart_buffer_tx);

	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_line("");
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("\n\r", spy_usart_buffer_tx);
}

/*! @brief Test print_string
 *	@param Group name
 *	@param Test name */
TEST(print, test_print_string) {
	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_string("this is test line");
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("this is test line", spy_usart_buffer_tx);

	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_string("");
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("", spy_usart_buffer_tx);
}

/*! @brief Test print_hex8
 *	@param Group name
 *	@param Test name */
TEST(print, test_print_hex8) {
	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_hex8(0x00);
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("0x00", spy_usart_buffer_tx);

	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_hex8(255);
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("0xFF", spy_usart_buffer_tx);

	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_hex8(100);
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("0x64", spy_usart_buffer_tx);
}

/*! @brief Test print_hex32
 *	@param Group name
 *	@param Test name */
TEST(print, test_print_hex32) {
	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_hex32(0x00);
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("0x00000000", spy_usart_buffer_tx);

	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_hex32(~0);
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("0xFFFFFFFF", spy_usart_buffer_tx);

	SET_SPY_OUTPUT();
	spy_usart_reset_tx();
	print_hex32(98000);
	SET_DEFAULT_OUTPUT();
	TEST_ASSERT_EQUAL_STRING("0x00017ED0", spy_usart_buffer_tx);
}

/*	@brief Set up all runnable tests from this module.
 *	@param group name.*/
TEST_GROUP_RUNNER(print) {
	RUN_TEST_CASE(print, test_print_char);
	RUN_TEST_CASE(print, test_print_line);
	RUN_TEST_CASE(print, test_print_string);
	RUN_TEST_CASE(print, test_print_hex8);
	RUN_TEST_CASE(print, test_print_hex32);
}
