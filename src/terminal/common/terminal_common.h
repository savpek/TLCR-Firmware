/*
 * terminal_common.h
 *
 * Created: 26.9.2011 22:08:03
 *  Author: savpek
 */ 


#ifndef TERMINAL_COMMON_H_
#define TERMINAL_COMMON_H_

/*!! Common functions for global use in subprograms. !!*/
extern errorc_t terminal_try_get_int_value (char* arg_str, uint32_t* return_arg_value);
extern errorc_t terminal_require_arg_str (char* arg_str);

/*@ XON/XOF protocol functions. Can be used to wait with data transmission. 
 * Sends pause and resume characters to compiler. */
extern void terminal_xon(void);
extern void terminal_xoff(void);

/*@ Send "Do you want continue? y/n" to terminal. Returns true if
 * user press 'y', otherwise false. */
extern errorc_t terminal_ask_continue(void);

/*@ Send "Are you sure? y/n" to terminal, returns true if 'y'. */
extern errorc_t terminal_ask_are_sure(void);

#endif /* TERMINAL_COMMON_H_ */