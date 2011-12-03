/*
 * subprog.h
 *
 * Created: 8.8.2011 20:24:01
 *  Author: savpek
 */ 


#ifndef SUBPROG_H_
#define SUBPROG_H_

#include "./errorh/inc/errorh.h"
#include "terminal.h"

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

/*! Subprograms to check, modify, write and control flash IC chip and storage data. !*/
/*@ Program checks that flash IC is responding for calls */
extern void subprog_flash_check(char* argument_str);

/*@ Prints selected segment data to terminal. */
extern void subprog_flash_print_segment(char* argument_str);

/*@ Prints selected segment data as text string to terminal */
extern void subprog_flash_print_segment_text(char* argument_str);

/*@ This program is for uploading entire flash IC content from text file.
 * see flash.txt for example (from root directory of project). */
extern void subprog_upload_flash_content(char* argument_str);

extern void subprog_flash_test_write(char* argument_str);
extern void subprog_flash_test_erase(char* argument_str);

/*@ Print helpfile to terminal */
extern void subprog_flash_print_help_file(char* argument_str);

#if 0
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
extern void subprog_ui_test(char* argument_str);
#endif //0
#endif /* SUBPROG_H_ */