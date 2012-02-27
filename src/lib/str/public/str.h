#ifndef CHARS_STRINGS_H_
#define CHARS_STRINGS_H_

#include "compiler.h"
#include "./errorh/inc/errorh.h"

/*! These helps you to make correctly sized buffers for strings. */
#define STR_HEX_UINT8_SIZE	5	// 0xFF\0
#define STR_HEX_UINT32_SIZE 11  // 0xFFFFFFFF\0
#define STR_DEC_UINT8_SIZE	4	// 255\0
#define STR_DEC_UINT32_SIZE 11	// 4294967295\0
#define STR_DEC_INT32_SIZE	12	// ?2147483648\0

/*! @brief Put source string to target string, insert begin from defined idx.
 *
 *	@param		source: Constant string that is inserted to another string.
 *	@param[our] target: Merge source string to this string. Beginning from begin_idx.
 *	@param		begin_idx: begin merge from this idx (target idx). */
extern void str_merge( char *source, char *target, uint8_t begin_idx);

/*! @brief Returns HEX string from uint8_t type number.
 *
 *	@param		input_value: (this is converted to HEX string).
 *	@param[out] *output_str:must be attleast 5 characters buffer! EG: 0xFF\0 */
extern void str_from_uint8_hex(uint8_t input_value, char *output_str);

/*! @brief Returns HEX string from uint32_t type number.
 *
 *	@param		input_value: (this is converted to HEX string).
 *	@param[out] *output_str: must be attleast 11 characters buffer! EG: 0xFFFFFFFF\0*/
extern void str_from_uint32_hex(uint32_t input_value, char *output_str);


/*! @brief Returns given value as string.
 *
 *	@param uint8_t input_value 	: This number is converted to string.
 *	@param char * output_str 	: Return string buffer.
 *
 * 	@return extern void: */
extern void str_from_uint8(uint8_t input_value, char *output_str);


/*! @brief Returns given value as string.
 *
 *	@param uint32_t input_value : This number is converted to string.
 *	@param char * output_str 	: Return string buffer.
 *
 * 	@return extern void: */
extern void str_from_uint32(uint32_t input_value, char *output_str);


/*! @brief Return numerical representation from value as string.
 *
 *	@param int32_t input_value 	: uint32_t type input value, converted to string.
 *	@param char * output_str 	: Return buffer. */
extern void str_from_int32(int32_t input_value, char *output_str);

/*!	@brief Compare two strings.
 *
 *	@param string_one: first string.
 *	@param string_two: second string.
 *	@param max_idx_to_compare: Last idx to check. This can be used to ignore compare after defined point.
 *	 Set max value to max_idx_to_compare (not used) by ~0.
 *
 *	@return EC_TRUE, EC_FALSE	*/
extern errorc_t str_compare(char *string_one, char *string_two, uint8_t max_idx_to_compare);

#endif /* CHARS_STRINGS_H_ */