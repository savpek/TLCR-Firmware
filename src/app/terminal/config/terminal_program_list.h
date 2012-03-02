/*
 * subprog.h
 *
 * Created: 8.8.2011 20:24:01
 *  Author: savpek
 */


#ifndef SUBPROG_H_
#define SUBPROG_H_

#include "./errorh/inc/errorh.h"
#include "./terminalapi/inc/terminalapi.h"

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
#define TERMINAL_PROGRAM_SCRIPT_ERASE

//#define TERMINAL_PROGRAM_FLASH_DUMP
//#define TERMINAL_PROGRAM_FLASH_ERASE

#define TERMINAL_PROGRAM_MOTOR_INIT
#define TERMINAL_PROGRAM_MOTOR_USE

#define TERMINAL_PROGRAM_MOVE
#define TERMINAL_PROGRAM_ROTATE
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
	extern void terminal_program_script_start(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_END
	extern void terminal_program_script_end(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_DELAY_MS
	extern void terminal_program_script_delay_ms(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_SHOW
	extern void terminal_program_script_show(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_UNDO
	extern void terminal_program_script_undo(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_ROTATE
	extern void terminal_program_script_rotate(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_MOVE
	extern void terminal_program_script_move(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_RUN
	extern void terminal_program_script_run(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_SCRIPT_ERASE
	extern void terminal_program_script_erase(terminalapi_cmd_t *cmd_struct);
#endif

/* TEST FUNCTIONS FOR MOTOR DRIVER */
#ifdef TERMINAL_PROGRAM_MOTOR_INIT
	extern void terminal_program_motor_init(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_MOTOR_USE
	extern void terminal_program_motor_use(terminalapi_cmd_t *cmd_struct);
#endif

/* DIRECT CONTROL OVER MOTION */
#ifdef TERMINAL_PROGRAM_MOVE
	extern void terminal_program_move(terminalapi_cmd_t *cmd_struct);
#endif
#ifdef TERMINAL_PROGRAM_ROTATE
	extern void terminal_program_rotate(terminalapi_cmd_t *cmd_struct);
#endif

#if 0
extern void subprog_ui_test(char* argument_str);
#endif //0
#endif /* SUBPROG_H_ */