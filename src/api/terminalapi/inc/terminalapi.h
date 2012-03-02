/*
 * terminalapi.h
 *
 * Created: 10.12.2011 11:53:36
 *  Author: savpek
 */


#ifndef TERMINALAPI_H_
#define TERMINALAPI_H_

#include "compiler.h"
#include "./str/public/str.h"
#include "./errorh/inc/errorh.h"

#include "./terminalapi/config/terminalapi_config.h"

/* Print functions */
void (*terminalapi_print)(char *string);
void (*terminalapi_put_char)(char character);
char (*terminalapi_get_char)(void);
errorc_t (*terminalapi_read_char)(char *ret_char);

/* This structure contains command and index
 * list of command string where certain points are.
 * For example: Where actual command ends and where
 * command value start. */
typedef struct
	{
	/* This is index of last char of actual command
	 * part */
	uint8_t cmd_end_idx;

	/* This is index of first char of cmd value from
	 * command string. */
	uint8_t value_begin_idx;

	/* This is index of last char of value. */
	uint8_t value_end_idx;

	/* This is EC_FALSE if CMD not has value,
	 * EC_TRUE if has. */
	errorc_t has_value_flag;

	/* This string contains hole typed command string */
	char command_str[TERMINALAPI_CMD_LENGTH];

	} terminalapi_cmd_t;

/* This structure contains list of runnable programs */
typedef struct
	{
	void (*program_pointer)(terminalapi_cmd_t *cmd_struct);
	char *command_string;
	} terminalapi_program_t;

/* Init terminal api, this must be called before any terminal api functions are used! */
extern void terminalapi_init(void);

/* This function read command from defined stream. Stays in until
 * user sends NL character. */
extern void terminalapi_get_command(terminalapi_cmd_t *cmd_struct);

/* This function try get int32 value from cmd string. Prints error
 * message if fails.
 * Return:	EC_FAILURE if there isn't value or it isn't number.
 *			EC_SUCCES if number returned successfully. */
extern errorc_t terminalapi_try_get_int32(terminalapi_cmd_t *cmd_struct, int32_t *ret_int_value);

/* This function requires that there are value string given.
 * Return:  EC_FALSE if no value string available.
			EC_TRUE if is.
 * Also prints error message to output stream. */
extern errorc_t terminalapi_check_value(terminalapi_cmd_t *cmd_struct);

/* This function tries run program (which are defined in terminalapi_program_list.h and .c).
 * Returns: EC_FAILURE if program doesn't excist.
 *			EC_SUCCESS if program is runned correctly. */
extern errorc_t terminalapi_try_run_program(terminalapi_cmd_t *cmd_struct, terminalapi_program_t program_list[]);

/* Ask "are you sure" question in terminal.
 * Returns: EC_TRUE, EC_FALSE */
extern errorc_t terminalapi_ask_sure( void );

/* Ask "do you want to continue" in terminal. */
extern errorc_t terminalapi_ask_continue( void );


#endif /* TERMINALAPI_H_ */