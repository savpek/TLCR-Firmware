/*!
 *	@file D:\Dropbox\AVR32\TLCR\FIRMWARE\src\api\read\test\test_read.c
 *  @author: savpek
 *
 *	@brief Test file for unity.
 */

#include "./utest/public/utest.h"

#include "./read/public/read.h"

#include "./usart/public/usart.h"
#include "./usart/spy/spy_usart.h"

char (*store_usart_read_char)(void);
char (*store_usart_try_read_char)(void);

/*! Notice that usart_put_char doesn't really put
 *	values in stream where usart get can get them!!!
 *	This behavior is only for made simulating easier. */

/*! @brief Set-up test group for: str
 *	@param Group name */
TEST_GROUP(read);

/* These functions are for setting and unsetting spy
 * for output stream. Needed to check read_line echo. */
static set_tx_spy() {
	spy_usart_reset_tx();
	backup_usart_putchar = usart_putchar;
	usart_putchar = spy_usart_putchar;
}

static unset_tx_spy() {
	usart_putchar = backup_usart_putchar;
}

/*! @brief Group setup function..
 *	@param Group name */
TEST_SETUP(read) {
	store_usart_read_char = usart_read_char;
	usart_read_char = spy_usart_read_char;

	store_usart_try_read_char = usart_try_read_char;
	usart_try_read_char = spy_usart_try_read_char;
}

/*! @brief Group teardown function..
 *	@param Group name */
TEST_TEAR_DOWN(read) {
	usart_read_char = store_usart_read_char;
	usart_try_read_char = store_usart_try_read_char;
}

/*! @brief test read_char().
 *	@param Group name
 *	@param Test name */
TEST(read, test_read_char) {
	spy_usart_put_to_rx("a");
	TEST_ASSERT_EQUAL_HEX8('a', read_char());
	/* This function is too complex to test practically
	 * because off functionality to "wait" as long as there
	 * are no correct kind of character in buffer ... */
}

/*! @brief test read_try_get_char().
 *	@param Group name
 *	@param Test name */
TEST(read, test_try_read_char) {
	/* Now simulated stream has char 'a' */
	spy_usart_put_to_rx("a");

	TEST_ASSERT_EQUAL_HEX8('a', read_try_get_char());
	TEST_ASSERT_EQUAL_HEX8('\0', read_try_get_char()); /* Empty */
	TEST_ASSERT_EQUAL_HEX8('\0', read_try_get_char()); /* Empty */

	spy_usart_put_to_rx("b");
	TEST_ASSERT_EQUAL_HEX8('b', read_try_get_char());
}

/*! @brief test read_try_get_char().
 *	@param Group name
 *	@param Test name */
TEST(read, test_read_data) {
	spy_usart_put_to_rx("\b");
	TEST_ASSERT_EQUAL_HEX8('\b', read_data());
}

/*! @brief test read_line().
 *	@param Group name
 *	@param Test name */
TEST(read, test_read_line) {
	char str[10] = {0};

	/* TEST CHARACTER RECEIVE OPERATIONS */

	/* Cannot test empty buffer here since
	 * read_line will get stuck in there forever. */

	/* Reversed order in buffer! This because true implementation
	 * of read stream buffer is FIFO. Spy driver isn't that smart ... */
	set_tx_spy();
	spy_usart_put_to_rx("\r\n\0x10");
	read_line(str, 5);
	unset_tx_spy();
	TEST_ASSERT_EQUAL_STRING("\r\n", spy_usart_buffer_tx); /* Check echo */
	TEST_ASSERT_EQUAL_STRING("", str); /* Should return empty string */

	set_tx_spy();
	spy_usart_put_to_rx("\r\na");
	read_line(str, 5);
	unset_tx_spy();
	TEST_ASSERT_EQUAL_STRING("a\r\n", spy_usart_buffer_tx);
	TEST_ASSERT_EQUAL_STRING("a", str);

	set_tx_spy();
	spy_usart_put_to_rx("\r\nab\bc");
	read_line(str, 10);
	unset_tx_spy();
	TEST_ASSERT_EQUAL_STRING("c\bba\r\n", spy_usart_buffer_tx);
	TEST_ASSERT_EQUAL_STRING("ba", str);

	set_tx_spy();
	spy_usart_put_to_rx("\na\177b"); /* Test delete (octal 177) */
	read_line(str, 5);
	unset_tx_spy();
	TEST_ASSERT_EQUAL_STRING("b\177a\r\n", spy_usart_buffer_tx);
	TEST_ASSERT_EQUAL_STRING("a", str);

	/* Test that backspace doesn't overrun index off limits.	*/
	set_tx_spy();
	spy_usart_put_to_rx("\n\b\b");
	read_line(str, 2);
	unset_tx_spy();
	TEST_ASSERT_EQUAL_STRING("\r\n", spy_usart_buffer_tx);
	TEST_ASSERT('\b' != str-1);

	/* Test that you cannot overrun index over high range */
	set_tx_spy();
	spy_usart_put_to_rx("\n\bbbb");
	read_line(str, 2); /* "b\n" */
	unset_tx_spy();
	TEST_ASSERT_EQUAL_STRING("bb\b\r\n", spy_usart_buffer_tx);
	TEST_ASSERT('b' != str[2]);
}

/*	@brief Set up all runnable tests from this module.
 *	@param group name.*/
TEST_GROUP_RUNNER(read) {
	RUN_TEST_CASE(read, test_read_char);
	RUN_TEST_CASE(read, test_try_read_char);
	RUN_TEST_CASE(read, test_read_data);
	RUN_TEST_CASE(read, test_read_line);
}