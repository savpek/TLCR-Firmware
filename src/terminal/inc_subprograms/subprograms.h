/*
 * subprog.h
 *
 * Created: 8.8.2011 20:24:01
 *  Author: savpek
 */ 


#ifndef SUBPROG_H_
#define SUBPROG_H_

#include "error_codes.h"
#include "terminal.h"

/*!! Common functions for global use in subprograms. !!*/
/*@ XON/XOF protocol functions. Can be used to wait with data transmission. 
 * Sends pause and resume characters to compiler. */
extern void subprogc_xon(void);
extern void subprogc_xoff(void);

/*@ Send "Do you want continue? y/n" to terminal. Returns true if
 * user press 'y', otherwise false. */
extern errorc_t subprogc_ask_continue(void);

/*@ Send "Are you sure? y/n" to terminal, returns true if 'y'. */
extern errorc_t subprogc_ask_are_sure(void);

/*!! Subprograms, designed to run through terminal service (terminal.h, terminal.c) !!*/
typedef const struct  
{
	const void (*pointer_uint32)(uint32_t params);
	const void (*pointer_str)(char *params);
	const char* command_str;
} subprog_list_t;

/*	This is the list of terminal sub-program functions.
 *	You must set subprograms in src_subprograms/list_of_subprograms.c */
extern subprog_list_t _subprog_array[];

/*!! Subprogram protypes, they should return VOID and take uint32_t or void
 *	 type as parameter !!*/
/*@ This program saves current settings to flash memory. After they
 * are saved program use them as default when started next time. */
extern void subprog_save_current_settings(void);
extern void subprog_set_speed(void);

/*! Subprograms to check, modify, write and control flash IC chip and storage data. !*/
/*@ Program checks that flash IC is responding for calls */
extern void subprog_flash_check(char* params);

/*@ Prints selected segment data to terminal. */
extern void subprog_flash_print_segment(uint32_t params);

/*@ Prints selected segment data as text string to terminal */
extern void subprog_flash_print_segment_text(uint32_t segment_id);

/*@ This program is for uploading entire flash IC content from text file.
 * see flash.txt for example (from root directory of project). */
extern void subprog_upload_flash_content(char* params);

extern void subprog_flash_test_write(uint32_t segment_id);
extern void subprog_flash_test_erase(char *params);

/*@ Print helpfile to terminal */
extern void subprog_flash_print_help_file(char* params);

/*!! Motion handle subprograms, these gives direct access
 *	 to motion handler libraries and motor control. */
extern void subprog_move_init(void);
extern void subprog_move_left(uint32_t mm_per_h);
extern void subprog_move_right(uint32_t mm_per_h);	
extern void subprog_move_stop(void);	
extern void subprog_move_exit(void);

extern void subprog_rotate_init(void);
extern void subprog_rotate_countercw(uint32_t mm_per_h);
extern void subprog_rotate_clockw(uint32_t mm_per_h);	
extern void subprog_rotate_stop(void);	
extern void subprog_rotate_exit(void);

extern void subprog_move_with_keyboard(uint32_t speed);

#endif /* SUBPROG_H_ */