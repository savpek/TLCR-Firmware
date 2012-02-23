/*
 * subprog_script.c
 *
 * Created: 6.12.2011 15:42:03
 *  Author: savpek
 */ 

#include "compiler.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"

/* Terminal internal */
#include "./app/terminal/config/terminal_program_list.h"
#include "./api/terminalapi/inc/terminalapi.h"

#include "./api/scriptapi/inc/scriptapi.h"
#include "./driver/usart/inc/usart.h"
#include "./api/chrstr/inc/chrstr.h"
#include "./api/motion/inc/motion.h"
#include "./api/storage/inc/storage.h"

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
		terminalapi_print("ERROR! Scripting sequence not active. Run script_start command!");
		return EC_FALSE;
		}
	else
		{
		return EC_TRUE;
		}
	}

void terminal_program_script_start(terminalapi_cmd_t *cmd_struct)
	{
	int32_t script_id = 0;
			
	/* Exit if given value is invalid. */
	if( !terminalapi_try_get_int32(cmd_struct, &script_id) ) return;
	
	/* Check that script exist with current id */
	if(SCRIPTAPI_SCRIPT_COUNT < script_id
		|| 0 > script_id )
		{
		terminalapi_print("Invalid script id!\n\r");
		return;
		}
	
	scriptapi_init_handle(&l_handle, script_id);
	
	l_active_flag = EC_TRUE;
	
	terminalapi_print("Scripting: Active!\n\r");
	
	/* TODO: Take mutex from scripting api */
	}
	
void terminal_program_script_end(terminalapi_cmd_t *cmd_struct)
	{
	terminalapi_print("Scripting: Inactive!\n\r");
	l_active_flag = EC_FALSE;
	
	/* TODO: Release mutex from scripting api */
	}

void terminal_program_script_delay_ms(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active()) return;
	
	int32_t cmd_value;
	
	if(!terminalapi_try_get_int32(cmd_struct, &cmd_value)) return;
	
	/* Write delay command to flash memory. */
	scriptapi_put_cmd(&l_handle, SCRIPTAPI_CMD_DELAY, cmd_value);
	}
		
void terminal_program_script_delay_s(terminalapi_cmd_t *cmd_struct)
	{
		
	}
	
void terminal_program_script_delay_h(terminalapi_cmd_t *cmd_struct)
	{
		
	}

void terminal_program_script_undo(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active()) return;
	
	uint32_t target_idx;
	target_idx = scriptapi_get_last_idx(&l_handle);
	
	/* Last index points location where isn't any data
	 * yet. So to point it last location where is data
	 * reduce by one. */
	
	int32_t cmd_temp = 0;
	int32_t value_temp = 0;
	
	while (EC_SUCCESS != scriptapi_get_cmd(&l_handle, target_idx, &cmd_temp, &value_temp))
		{
		/* Check for beginning of the script. */
		if(0 == target_idx)
			{
			terminalapi_print("Nothing to undo!\r\n");
			return;	
			}
		target_idx--;
		}
		
	scriptapi_clr_cmd(&l_handle, target_idx);
	
	terminalapi_print("Last line removed!\n\r");
	}

void terminal_program_script_status(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active())
		{
		terminalapi_print("Scripting not active!\r\n");
		}
	else
		{
		terminalapi_print("Scripting active!");	
		}
	}

/* Prints script from beginning */
void terminal_program_script_show(terminalapi_cmd_t *cmd_struct)
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
		terminalapi_print(command_array[cmd_temp%4]);
		terminalapi_print(": ");
		
		chrstr_int32_to_dec_str(value_temp, string_buffer);
		terminalapi_print(string_buffer);
		
		terminalapi_print(" ");
		terminalapi_print(unit_array[cmd_temp%4]);
		
		terminalapi_print(" \r\n");
		}
	}
	
void terminal_program_script_show_current(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active()) return;	
	}
	
void terminal_program_script_move(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active()) return;
	
	int32_t cmd_value = 0;
	
	if(!terminalapi_try_get_int32(cmd_struct, &cmd_value)) return;
	
	/* Write delay command to flash memory. */
	scriptapi_put_cmd(&l_handle, SCRIPTAPI_CMD_MOVE, cmd_value);	
	}
	
void terminal_program_script_rotate(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active()) return;
	
	int32_t cmd_value;
	
	if(!terminalapi_try_get_int32(cmd_struct, &cmd_value)) return;
	
	/* Write delay command to flash memory. */
	scriptapi_put_cmd(&l_handle, SCRIPTAPI_CMD_ROTATE, cmd_value);
	}
	
void terminal_program_script_run(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active()) return;
	int32_t cmd_value;
	scriptapi_cmd_t cmd;
	uint32_t cmd_idx = 0;
	
	portTickType rtos_last_wake_time = xTaskGetTickCount();
	
	/* TODO: ADD ESC BUTTON TO QUIT SCRIPT! */
	while( EC_EMPTY != scriptapi_get_cmd(&l_handle, cmd_idx, &cmd, &cmd_value) )
		{
		cmd_idx++;
		
		switch(cmd)
			{
			case SCRIPTAPI_CMD_DELAY:
				vTaskDelayUntil(&rtos_last_wake_time, (cmd_value/portTICK_RATE_MS));
				break;

			case SCRIPTAPI_CMD_EMPTY:
				break;
				
			case SCRIPTAPI_CMD_ROTATE:
				terminalapi_print("CMD_ROTATE\n\r");
				motion_rotate(cmd_value, MOTION_ACCESS_TERMINAL);
				break;
				
			case SCRIPTAPI_CMD_MOVE:
				terminalapi_print("CMD_MOVE\n\r");
				motion_move(cmd_value, MOTION_ACCESS_TERMINAL);
				break;
			}
		}
	
	/* Stop motors after run script */
	motion_rotate(0, MOTION_ACCESS_TERMINAL);
	motion_move(0, MOTION_ACCESS_TERMINAL);
	
	terminalapi_print("Script ended!\r\n");
	}
	
void terminal_program_script_erase(terminalapi_cmd_t *cmd_struct)
	{
	if(!s_is_scripting_active()) return;
	
	storage_erase_segment(l_handle.storage_segment_id);
	
	/* Reset handle (current script deleted, return to begin). */
	scriptapi_init_handle(&l_handle, l_handle.script_id );
	
	terminalapi_print("Script erased!\r\n");
	}