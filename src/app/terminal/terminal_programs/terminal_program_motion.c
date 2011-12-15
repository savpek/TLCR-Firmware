/*
 * terminal_program_motion.c
 *
 * Created: 13.12.2011 12:43:39
 *  Author: savpek
 */ 

/* Terminal internal */
#include "./app/terminal/config/terminal_program_list.h"
#include "./api/terminalapi/inc/terminalapi.h"

#include "./api/motion/inc/motion.h"

void terminal_program_rotate(terminalapi_cmd_t *cmd_struct)
	{
	int32_t rotate_speed_md = 0;
	
	if(!terminalapi_try_get_int32(cmd_struct, &rotate_speed_md)) return;
	
	if(EC_SUCCESS != motion_rotate(rotate_speed_md, MOTION_ACCESS_TERMINAL))
		{
		terminalapi_print("Error! Cannot take control over motion!");	
		}
	}
	
void terminal_program_move(terminalapi_cmd_t *cmd_struct)
	{
	int32_t move_speed_um = 0;
	
	if(!terminalapi_try_get_int32(cmd_struct, &move_speed_um)) return;
	
	if(EC_SUCCESS != motion_move(move_speed_um, MOTION_ACCESS_TERMINAL))
		{
		terminalapi_print("Error! Cannot take control over motion!");	
		}
	}