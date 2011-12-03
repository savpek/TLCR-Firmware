/*
 * terminal_common.h
 *
 * Created: 26.9.2011 22:08:03
 *  Author: savpek
 */ 


#ifndef TERMINAL_COMMON_H_
#define TERMINAL_COMMON_H_

/* ASF */
#include "compiler.h"

/* TLCR libs */
#include "./errorh/inc/errorh.h"

/* Tests inside of subprogram that arguments are correct type or that they are given. 
 * Usage example: if (! terminal_require_arg_str(str)) return;, exits current subprogram
 * if no argument is given. Functions also outputs error message to defined USART. */
extern errorc_t terminal_try_get_int_value (char* arg_str, int32_t* return_arg_value);
extern errorc_t terminal_require_arg_str (char* arg_str);

/*@ Send "Do you want continue? y/n" to terminal. Returns EC_TRUE if
 * user press 'y', otherwise EC_FALSE. */
extern errorc_t terminal_ask_continue(void);

/*@ Send "Are you sure? y/n" to terminal, returns EC_TRUE if 'y'. */
extern errorc_t terminal_ask_are_sure(void);

#endif /* TERMINAL_COMMON_H_ */