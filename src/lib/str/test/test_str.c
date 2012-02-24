/*!	@brief Test STR library.
 *	@file test_str.c
 *
 *  @author: savpek
 */

#include "./utest/public/utest.h"

#include "./str/public/str.h"

/*! @brief Set-up test group for: str
 *	@param Group name */
TEST_GROUP(str);

/*! @brief Group setup function..
 *	@param Group name */
TEST_SETUP(str) {

}

/*! @brief Group teardown function..
 *	@param Group name */
TEST_TEAR_DOWN(str) {

}

/*! @brief Test compare functionality.
 *	@param Group name
 *	@param Test name */
TEST(str, test_str_compare) {
	TEST_ASSERT( (str_compare("abc", "abc", ~0) == EC_TRUE) );
	TEST_ASSERT( (str_compare("abc", "acb", ~0) == EC_FALSE) );
	TEST_ASSERT( (str_compare("", "", ~0) == EC_TRUE) );
	TEST_ASSERT( (str_compare("abcd", "", ~0) == EC_FALSE) );
	TEST_ASSERT( (str_compare("", "abcd", ~0) == EC_FALSE) );
	TEST_ASSERT( (str_compare("abc", "abcd", ~0) == EC_FALSE) );
	TEST_ASSERT( (str_compare("abcd", "abc", ~0) == EC_FALSE) );
	TEST_ASSERT( (str_compare("abcd", "abc", 2) == EC_TRUE) );
	TEST_ASSERT( (str_compare("abcdef", "abc", 3) == EC_FALSE) );
}

/*! @brief Test uint8->hex functionality.
 *	@param Group name
 *	@param Test name */
TEST(str, test_str_from_uint8_hex) {
	volatile char str_buffer[30] = {0};

	str_from_uint8_hex(0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0x00", str_buffer);

	str_from_uint8_hex(~0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0xFF", str_buffer);
	
	str_from_uint8_hex(20, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0x14", str_buffer);
}

/*! @brief Test uint32->hex functionality.
 *	@param Group name
 *	@param Test name */
TEST(str, test_str_from_uint32_hex) {
	volatile char str_buffer[30] = {0};

	str_from_uint32_hex(0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0x00000000", str_buffer);

	str_from_uint32_hex(~0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0xFFFFFFFF", str_buffer);
	
	str_from_uint32_hex(20, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0x00000014", str_buffer);
}

/*! @brief Test uint8->dec string functionality.
 *	@param Group name
 *	@param Test name */
TEST(str, test_str_from_uint8) {
	volatile char str_buffer[30] = {0};

	str_from_uint8(0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0", str_buffer);
	
	str_from_uint8(~0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("255", str_buffer);
	
	str_from_uint8(255+10, str_buffer);
	TEST_ASSERT_EQUAL_STRING("9", str_buffer);
}

/*! @brief Test uint32->dec string functionality.
 *	@param Group name
 *	@param Test name */
TEST(str, test_str_from_uint32) {
	volatile char str_buffer[30] = {0};

	str_from_uint32(0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0", str_buffer);
	
	str_from_uint32(~0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("4294967295", str_buffer);
	
	str_from_uint32(3500, str_buffer);
	TEST_ASSERT_EQUAL_STRING("3500", str_buffer);
	
	str_from_uint32(4294967295+10, str_buffer);
	TEST_ASSERT_EQUAL_STRING("9", str_buffer);
}

/*! @brief Test uint32->dec string functionality.
 *	@param Group name
 *	@param Test name */
TEST(str, test_str_from_int32) {
	volatile char str_buffer[30] = {0};

	str_from_int32(0, str_buffer);
	TEST_ASSERT_EQUAL_STRING("0", str_buffer);
	
	str_from_int32(-100, str_buffer);
	TEST_ASSERT_EQUAL_STRING("-100", str_buffer);

	str_from_int32(100, str_buffer);
	TEST_ASSERT_EQUAL_STRING("100", str_buffer);

	str_from_int32(-2147483648, str_buffer);
	TEST_ASSERT_EQUAL_STRING("-2147483648", str_buffer);
	
	str_from_int32(2147483647, str_buffer);
	TEST_ASSERT_EQUAL_STRING("2147483647", str_buffer);
	
	str_from_int32(2147483647+10, str_buffer);
	TEST_ASSERT_EQUAL_STRING("-2147483639", str_buffer);
	
	str_from_int32(-2147483648-10, str_buffer);
	TEST_ASSERT_EQUAL_STRING("2147483638", str_buffer);
}

/*! @brief Test compare functionality.
 *	@param Group name
 *	@param Test name */
TEST(str, test_str_merge) {
	volatile char str_bufferx[30] = {0};

	str_merge("merged", str_bufferx, 0);
	TEST_ASSERT_EQUAL_STRING("merged", str_bufferx);
	str_merge(" merged", str_bufferx, 6);
	TEST_ASSERT_EQUAL_STRING("merged merged", str_bufferx);
}

/* @brief Set up all runnable tests from this module. */
TEST_GROUP_RUNNER(str) {
	RUN_TEST_CASE(str, test_str_compare);
	RUN_TEST_CASE(str, test_str_from_uint8_hex);
	RUN_TEST_CASE(str, test_str_merge);
	RUN_TEST_CASE(str, test_str_from_uint32_hex);
	RUN_TEST_CASE(str, test_str_from_uint32);
	RUN_TEST_CASE(str, test_str_from_uint8);
	RUN_TEST_CASE(str, test_str_from_int32);
}