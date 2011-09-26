/*
 * list_of_subprograms.c
 *
 * Created: 24.8.2011 1:31:04
 *  Author: savpek
 */ 

#include "compiler.h"
#include "subprograms.h"

/*!! Array of runnable programs, remember to put program in correct
 *  category. Category tells is program parameter int or string. !*/
subprog_list_t _subprog_array[] =
	   {		
		/*! Remember to put program in correct category. Category tells type
		 *	of parameter function takes. */
		
		/*POINTER PARAM:UINT32_t	 |	POINTER PARAM:CHAR*		|		Command STR		|*/
		{ 0,							&subprog_flash_check,			"flash_check"},
		{ &subprog_flash_print_segment,	0,								"flash_read_hex"},			
		{ &subprog_flash_print_segment_text,0,							"flash_read"},
		{ 0,							&subprog_upload_flash_content,	"flash_rewrite"},
		{ &subprog_flash_test_write,	0							,	"flash_test_write"},
		{ 0							,	&subprog_flash_print_help_file,	"help"},
		
		/*! MOTION subprograms, XMOTOR !*/
		{
			.pointer_uint32 = 0,
			.pointer_str = &subprog_move_init,
			.command_str = "move_init"
		},
		{
			.pointer_uint32 = &subprog_move_left,
			.pointer_str = 0,
			.command_str = "move_left"	
		},
		{
			.pointer_uint32 = &subprog_move_right,
			.pointer_str = 0,
			.command_str = "move_right"	
		},
		{
			.pointer_uint32 = 0,
			.pointer_str = &subprog_move_stop,
			.command_str = "move_stop"
		},
		{
			.pointer_uint32 = 0,
			.pointer_str = &subprog_move_exit,
			.command_str = "move_exit"
		},
		/*! MOTION subprograms, RMOTOR !*/
		{
			.pointer_uint32 = 0,
			.pointer_str = &subprog_rotate_init,
			.command_str = "rotate_init"
		},
		{
			.pointer_uint32 = &subprog_rotate_countercw,
			.pointer_str = 0,
			.command_str = "rotate_countercw"	
		},
		{
			.pointer_uint32 = &subprog_rotate_clockw,
			.pointer_str = 0,
			.command_str = "rotate_countercw"	
		},
		{
			.pointer_uint32 = 0,
			.pointer_str = &subprog_rotate_stop,
			.command_str = "rotate_stop"
		},
		{
			.pointer_uint32 = 0,
			.pointer_str = &subprog_rotate_exit,
			.command_str = "rotate_exit"
		},
		/* ! Program that allows control device with ASDW keys */
		{
			.pointer_uint32 = &subprog_move_with_keyboard,
			.pointer_str = 0,
			.command_str = "move_with_keyboard"
		},
		{ 0,							0,								0}
		/* Last member of subprogram array must be null! */
	   };
