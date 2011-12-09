/*
 * subprog_script.c
 *
 * Created: 6.12.2011 15:42:03
 *  Author: savpek
 */ 

#include "compiler.h"

/* Terminal internal */
#include "./app/terminal/subprograms/config/subprogram_list.h"
#include "./app/terminal/inc/terminal.h"
#include "./app/terminal/common/common_functions/inc/terminal_common_functions.h"

#include "./api/scriptapi/inc/scriptapi.h"
#include "./driver/usart/inc/usart.h"
#include "./api/chrstr/inc/chrstr.h"


typedef struct	 
	{
	/* Is scripting sequence active (started with script_start command) */
	errorc_t active;
	
	/* Current script id, defines segment where program will be writed down. */
	uint32_t cur_script_id;
	
	/* Index of current command */
	uint32_t cur_cmd_idx;
	
	} subprog_script_status_t;

/* This tells to subprograms that is scripting
 * sequncy currently active (script id defined) */
volatile errorc_t l_active_flag = EC_FALSE; 
	
/* This handle struct type is defined in scriptapi. ! */
volatile scriptapi_t l_handle;
	
/* This function checks that current script is active,
 * if not prints error message to terminal and returns EC_FALSE */
static errorc_t s_is_scripting_active( void )
	{
	if(l_active_flag != EC_TRUE)
		{
		usart_write_line("ERROR! Scripting sequence not active. Run script_start command!");
		return EC_FALSE;
		}
	else
		{
		return EC_TRUE;
		}
	}

void subprog_script_start(char *arg_str)
	{
	uint32_t script_id = 0;
			
	/* Exit if given value is invalid. */
	if( !terminal_try_get_int_value(arg_str, &script_id) ) return;
	
	/* Check that script exist with current id */
	if(SCRIPTAPI_SCRIPT_COUNT < script_id
		|| 0 > script_id )
		{
		usart_write_line("Invalid script id!\n\r");
		return;
		}
	
	scriptapi_init_handle(&l_handle, script_id);
	
	l_active_flag = EC_TRUE;
	
	usart_write_line("Scripting: Active!\n\r");
	
	/* TODO: Take mutex from scripting api */
	}
	
void subprog_script_end(char *arg_str)
	{
	usart_write_line("Scripting: Inactive!\n\r");
	l_active_flag = EC_FALSE;
	
	/* TODO: Release mutex from scripting api */
	}

void subprog_script_delay_ms(char *arg_str)
	{
	if(!s_is_scripting_active()) return;
	
	int32_t cmd_value;
	
	if(!terminal_try_get_int_value(arg_str, &cmd_value)) return;
	
	/* Write delay command to flash memory. */
	scriptapi_put_cmd(&l_handle, SCRIPTAPI_CMD_DELAY, cmd_value);
	}
		
void subprog_script_delay_s(char *arg_str)
	{
		
	}
	
void subprog_script_delay_h(char *arg_str)
	{
		
	}

void subprog_script_undo(char *arg_str)
	{
	if(!s_is_scripting_active()) return;
	
	uint32_t target_idx;
	target_idx = scriptapi_get_last_idx(&l_handle);
	
	/* Last index points location where isn't any data
	 * yet. So to point it last location where is data
	 * reduce by one. */
	
	int32_t cmd_temp = 0;
	int32_t value_temp = 0;

	/* Script is empty */
	if(1 == target_idx)
		{
		usart_write_line("Nothing to undo!\r\n");
		return;	
		}
	
	do 
		{
		target_idx--;
		} while (EC_SUCCESS != scriptapi_get_cmd(&l_handle, target_idx, &cmd_temp, &value_temp));
		
	scriptapi_clr_cmd(&l_handle, target_idx);
	
	usart_write_line("Last line removed!\n\r");
	}

void subprog_script_status(char *arg_str)
	{
	if(!s_is_scripting_active())
		{
		usart_write_line("Scripting not active!\r\n");
		}
	else
		{
		usart_write_line("Scripting active!");	
		}
	}

/* Prints script from beginning */
void subprog_script_show(char *arg_str)
	{
	if(!s_is_scripting_active()) return;
	
	const char command_array[4][7] = 
		{
		"EMPTY ",
		"DELAY ",
		"MOVE  ",
		"ROTATE"
		};
	
	const char unit_array[4][5] =
		{
		"    ",
		"ms  ",
		"um/h",
		"mD/h"
		};
		
	uint32_t current_idx = 0;
	
	int32_t cmd_temp;
	int32_t value_temp;
	
	/* Buffer for printing numerical values */
	uint8_t string_buffer[CHRSTR_DEC_U32_SIZE] = {0};
	
	while(scriptapi_get_cmd(&l_handle, current_idx, &cmd_temp, &value_temp) != EC_EMPTY)
		{
		current_idx++;
		
		/* If empty command, dont print. */
		if(cmd_temp == SCRIPTAPI_CMD_EMPTY) continue;
		
		/* If is actual command, print it */
		usart_write_line(command_array[cmd_temp%4]);
		usart_write_line(": ");
		
		chrstr_int32_to_dec_str(value_temp, string_buffer);
		usart_write_line(string_buffer);
		
		usart_write_line(" ");
		usart_write_line(unit_array[cmd_temp%4]);
		
		usart_write_line(" \r\n");
		}
	}
	
void subprog_script_show_current(char *arg_str)
	{
	if(!s_is_scripting_active()) return;	
	}
	
void subprog_script_move(char *arg_str)
	{
	if(!s_is_scripting_active()) return;
	
	int32_t cmd_value;
	
	if(!terminal_try_get_int_value(arg_str, &cmd_value)) return;
	
	/* Write delay command to flash memory. */
	scriptapi_put_cmd(&l_handle, SCRIPTAPI_CMD_MOVE, cmd_value);	
	}
	
void subprog_script_rotate(char *arg_str)
	{
	if(!s_is_scripting_active()) return;
	
	int32_t cmd_value;
	
	if(!terminal_try_get_int_value(arg_str, &cmd_value)) return;
	
	/* Write delay command to flash memory. */
	scriptapi_put_cmd(&l_handle, SCRIPTAPI_CMD_ROTATE, cmd_value);
	}

