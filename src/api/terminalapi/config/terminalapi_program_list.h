/*
 * subprog.h
 *
 * Created: 8.8.2011 20:24:01
 *  Author: savpek
 */ 


#ifndef SUBPROG_H_
#define SUBPROG_H_

#include "./api/errorh/inc/errorh.h"
#include "./app/terminal/inc/terminal.h"

/*!! Subprograms, designed to run through terminal service (terminal.h, terminal.c) !!*/
typedef const struct  
{
	const void (*pointer)(char *params);
	const char* command_str;
} subprog_list_t;

/*	This is the list of terminal sub-program functions.
 *	You must set subprograms in src_subprograms/list_of_subprograms.c */
extern subprog_list_t _subprog_array[];

/*!! Subprogram protypes, they should return VOID and take char* argument_str
 *   as only parameter. !!*/

extern void subprog_test_get_int(char* arg_str);

/* FLASH SUBPROGRAMS */
/* Command flash_dump */
extern void subprog_flash_dump(char* arg_str);
extern void subprog_flash_erase(char *arg_str);

extern void subprog_script_start(char *arg_str);
extern void subprog_script_end(char *arg_str);
extern void subprog_script_delay_ms(char *arg_str);
extern void subprog_script_show(char *arg_str);
extern void subprog_script_undo(char *arg_str);
extern void subprog_script_rotate(char *arg_str);
extern void subprog_script_move(char *arg_str);

/*!! Motion handle subprograms, these gives direct access
 *	 to motion handler libraries and motor control. */
extern void subprog_move_init(char* argument_str);
extern void subprog_move_left(char* argument_str);
extern void subprog_move_right(char* argument_str);	
extern void subprog_move_stop(char* argument_str);	
extern void subprog_move_exit(char* argument_str);

extern void subprog_rotate_init(char* argument_str);
extern void subprog_rotate_countercw(char* argument_str);
extern void subprog_rotate_clockw(char* argument_str);	
extern void subprog_rotate_stop(char* argument_str);	
extern void subprog_rotate_exit(char* argument_str);

extern void subprog_move_with_keyboard(char* argument_str);

#if 0
extern void subprog_ui_test(char* argument_str);
#endif //0
#endif /* SUBPROG_H_ */