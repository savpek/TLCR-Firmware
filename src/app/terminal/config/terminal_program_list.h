/*
 * subprog.h
 *
 * Created: 8.8.2011 20:24:01
 *  Author: savpek
 */ 


#ifndef SUBPROG_H_
#define SUBPROG_H_

#include "./api/errorh/inc/errorh.h"
#include "./api/terminalapi/inc/terminalapi.h"

/*!! Undefine these if you dont want include subprogram to current build. !!*/
/* Scripting programs */
#define TERMINAL_PROGRAM_SCRIPT_START
#define TERMINAL_PROGRAM_SCRIPT_END
#define TERMINAL_PROGRAM_SCRIPT_RUN
#define TERMINAL_PROGRAM_SCRIPT_DELAY_MS
#define TERMINAL_PROGRAM_SCRIPT_SHOW
#define TERMINAL_PROGRAM_SCRIPT_UNDO
#define TERMINAL_PROGRAM_SCRIPT_ROTATE
#define TERMINAL_PROGRAM_SCRIPT_MOVE

//#define TERMINAL_PROGRAM_FLASH_DUMP
//#define TERMINAL_PROGRAM_FLASH_ERASE

//#define TERMINAL_PROGRAM_MOVE
//#define TERMINAL_PROGRAM_ROTATE
//#define TERMINAL_PROGRAM_KEYBOARD_CONTROL

/*	This is the list of terminal sub-program functions.
 *	You must set subprograms in src_subprograms/list_of_subprograms.c */
extern terminalapi_program_t terminal_program_array[];

/*!! Subprogram protypes, they should return VOID and take char* argument_str
 *   as only parameter. !!*/

/* FLASH */
#ifdef TERMINAL_PROGRAM_FLASH_DUMP
	extern void subprog_flash_dump(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_FLASH_ERASE
	extern void subprog_flash_erase(terminalapi_cmd_t *cmd_struct);
#endif

/* SCRIPT */
#ifdef TERMINAL_PROGRAM_SCRIPT_START
	extern void subprog_script_start(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_END
	extern void subprog_script_end(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_DELAY_MS
	extern void subprog_script_delay_ms(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_SHOW
	extern void subprog_script_show(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_UNDO
	extern void subprog_script_undo(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_ROTATE
	extern void subprog_script_rotate(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_MOVE
	extern void subprog_script_move(terminalapi_cmd_t *cmd_struct);
#endif

/* DIRECT CONTROL OVER MOTION */


#if 0
extern void subprog_ui_test(char* argument_str);
#endif //0
#endif /* SUBPROG_H_ */