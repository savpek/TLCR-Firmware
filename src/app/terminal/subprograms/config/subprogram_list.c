/*
 * list_of_subprograms.c
 *
 * Created: 24.8.2011 1:31:04
 *  Author: savpek
 */ 

#include "compiler.h"
#include "subprogram_list.h"

/*!! Array of runnable programs, you must define function
 *	 pointer and command string that calls function from terminal !*/
subprog_list_t _subprog_array[] =
	   {
		{ &subprog_test_get_int,				"test_get_int"		},
		#if 0			
		/*POINTER;								| CMD string		*/
		/*! FLASH subprograms */
		{ &subprog_flash_check,					"flash_check"		},
		{ &subprog_flash_print_segment,			"flash_read_hex"	},			
		{ &subprog_flash_print_segment_text,	"flash_read"		},
		{ &subprog_upload_flash_content,		"flash_rewrite"		},
		{ &subprog_flash_test_write,			"flash_test_write"	},
		{ &subprog_flash_print_help_file,		"help"				},
		

		/*! MOTION subprograms, XMOTOR !*/
		{ &subprog_move_init,					"move_init"			},
		{ &subprog_move_left,					"move_left"			},
		{ &subprog_move_right,					"move_right"		},
		{ &subprog_move_stop,					"move_stop"			},
		{ &subprog_move_exit,					"move_exit"			},
		/*! MOTION subprograms, RMOTOR !*/
		{ &subprog_rotate_init,					"rotate_init"		},
		{ &subprog_rotate_countercw,			"rotate_countercw"	},
		{ &subprog_rotate_clockw,				"rotate_countercw"	},
		{ &subprog_rotate_stop,					"rotate_stop"		},
		{ &subprog_rotate_exit,					"rotate_exit"		},
		{ &subprog_ui_test	,					"ui_test"			},
		
		/* ! Program that allows control device with ASDW keys */
		{ &subprog_move_with_keyboard,			"move_with_keyboard"},
		#endif
		/* Last member of subprogram array must be null! */
		{ 0,									0}
		
	   };
