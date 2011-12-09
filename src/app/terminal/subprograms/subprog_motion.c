/*
 * subprog_motion.c
 *
 * Created: 30.8.2011 23:11:51
 *  Author: savpek
 */ 
#include "compiler.h"

#include "./app/terminal/subprograms/config/subprogram_list.h"
#include "./api/motion/inc/motion.h"
#include "./app/terminal/common/common_functions/inc/terminal_common_functions.h"
#include "./driver/usart/inc/usart.h"

void subprog_move_init(char* argument_str)
	{
	usart_write_line("Terminal control over XMOTOR.\r\n");
	motion_force_take_control(MOTION_XMOTOR_ID, TERMINAL);	
	}

void subprog_move_left(char* argument_str)
	{
	uint32_t mm_per_h = 0;
		
	/* If parameter given for subprogram isn't number, exit program */
	if(terminal_try_get_int_value(argument_str, &mm_per_h) != EC_SUCCESS) return;

	if(motion_check_control(MOTION_XMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");
		return;
		}
	
	motion_set_direction(MOTION_XMOTOR_ID, STOP);
	motion_set_move_speed(MOTION_XMOTOR_ID, mm_per_h);
	motion_set_direction(MOTION_XMOTOR_ID, LEFT);
	}
	
void subprog_move_right(char* argument_str)
	{
	uint32_t mm_per_h = 0;
		
	/* If parameter given for subprogram isn't number, exit program */
	if(terminal_try_get_int_value(argument_str, &mm_per_h) != EC_SUCCESS) return;
			
	if(motion_check_control(MOTION_XMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");
		return;
		}
		
	motion_set_direction(MOTION_XMOTOR_ID, STOP);
	motion_set_move_speed(MOTION_XMOTOR_ID, mm_per_h);
	motion_set_direction(MOTION_XMOTOR_ID, RIGHT);	
	}
	
void subprog_move_stop(char* argument_str)
	{
	if(motion_check_control(MOTION_XMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");	
		return;
		}
		
	usart_write_line("Motor stopped.");
	motion_set_direction(MOTION_XMOTOR_ID, STOP);	
	}
	
void subprog_move_exit(char* argument_str)
	{
	if(motion_check_control(MOTION_XMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");	
		return;
		}
	
	usart_write_line("Released XMOTOR handle.");
	motion_release_control(MOTION_XMOTOR_ID);
	}
	
void subprog_rotate_init(char* argument_str)
	{
	usart_write_line("Terminal control over RMOTOR.\r\n");
	motion_force_take_control(MOTION_RMOTOR_ID, TERMINAL);	
	}

void subprog_rotate_countercw(char* argument_str)
	{
	uint32_t hundred_of_angle_per_h = 0;
		
	/* If parameter given for subprogram isn't number, exit program */
	if(terminal_try_get_int_value(argument_str, &hundred_of_angle_per_h) != EC_SUCCESS) return;

	if(motion_check_control(MOTION_RMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");
		return;
		}
	
	motion_set_direction(MOTION_RMOTOR_ID, STOP);
	motion_set_move_speed(MOTION_RMOTOR_ID, hundred_of_angle_per_h);
	motion_set_direction(MOTION_RMOTOR_ID, LEFT);
	}
	
void subprog_rotate_clockw(char* argument_str)
	{
	uint32_t hundred_of_angle_per_h = 0;
		
	/* If parameter given for subprogram isn't number, exit program */
	if(terminal_try_get_int_value(argument_str, &hundred_of_angle_per_h) != EC_SUCCESS) return;
		
	if(motion_check_control(MOTION_RMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");
		return;
		}
			
	motion_set_direction(MOTION_RMOTOR_ID, STOP);
	motion_set_move_speed(MOTION_RMOTOR_ID, hundred_of_angle_per_h);
	motion_set_direction(MOTION_RMOTOR_ID, RIGHT);	
	}
	
void subprog_rotate_stop(char* argument_str)
	{
	if(motion_check_control(MOTION_RMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");	
		return;
		}
		
	usart_write_line("Motor stopped.");
	motion_set_direction(MOTION_RMOTOR_ID, STOP);	
	}
	
void subprog_rotate_exit(char* argument_str)
	{
	if(motion_check_control(MOTION_RMOTOR_ID) != TERMINAL)
		{
		usart_write_line("Take control of XMOTOR first. \r\n");	
		return;
		}
	
	usart_write_line("Released XMOTOR handle.");
	motion_release_control(MOTION_RMOTOR_ID);
	}
	
void subprog_move_with_keyboard(char* argument_str)
	{
	uint32_t speed = 0;
	
	/* If parameter given for subprogram isn't number, exit program */
	if(terminal_try_get_int_value(argument_str, &speed) != EC_SUCCESS) return;
		
	char received_char = 0;
	
	/* Set speed for movements: */
	motion_set_move_speed(0, speed);
	motion_set_move_speed(1, speed);
	
	/* Take control over motors */
	motion_force_take_control(0, TERMINAL);
	motion_force_take_control(1, TERMINAL);
	
	/* Print program info */
	usart_write_line("Control device with A,S,D,W keys. Press ESC if you want to quit!");
	
	while((received_char = usart_getchar()) != 0x1B)
		{
		motion_set_direction(0, STOP);
		motion_set_direction(1, STOP);
			
		/* Move device left */
		if(received_char == 'a')
			{
			motion_set_direction(MOTION_XMOTOR_ID, LEFT);
			}
		/* Move device right */
		else if (received_char == 'd')
			{
			motion_set_direction(MOTION_XMOTOR_ID, RIGHT);
			}
		/* Rotate counter clockwise */
		else if (received_char == 'w')
			{
			motion_set_direction(MOTION_RMOTOR_ID, COUNTER_CLOCKW);
			}
		/* Rotate clockwise */			
		else if (received_char == 's')
			{
			motion_set_direction(MOTION_RMOTOR_ID, CLOCKW);	
			}
		}	

	motion_set_direction(0, STOP);	
	motion_set_direction(1, STOP);
	}