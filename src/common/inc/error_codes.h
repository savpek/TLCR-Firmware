/*
 * error_codes.h
 *
 * Created: 3.8.2011 12:21:34
 *  Author: savpek
 */ 


#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_

typedef enum 
	{
	EC_PASS = 1,
	EC_SUCCES = 1,
	EC_DONE = 1,
	EC_TRUE = 1,
	EC_NULL = 0,
	EC_FAIL = 0,
	EC_FALSE = 0,
	EC_FULL = 1,
	EC_NOT_EMPTY = 2,
	EC_LOCKED = 3,
	EC_TIMEOUT = 4,
	EC_OUT_OF_RANGE = 5,
	EC_NOT_NUMBER = 6,
	EC_VALUE_ERROR = 7,
	EC_GOTO_NEXT = 8,
	EC_END = 9,
	EC_EXIT = 10
	}	errorc_t;

#endif /* ERROR_CODES_H_ */