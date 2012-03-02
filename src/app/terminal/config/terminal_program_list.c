/*
 * list_of_subprograms.c
 *
 * Created: 24.8.2011 1:31:04
 *  Author: savpek
 */ 

#include "compiler.h"
#include "./app/terminal/config/terminal_program_list.h"
#include "./terminalapi/inc/terminalapi.h"

/*!! Array of runnable programs, you must define function
 *	 pointer and command string that calls function from terminal !*/
terminalapi_program_t terminal_program_array[] =
	   {
		/* FLASH */
		#ifdef TERMINAL_PROGRAM_FLASH_DUMP
		{ &subprog_flash_dump,				"flash_dump"		},
		#endif
		#ifdef TERMINAL_PROGRAM_FLASH_ERASE
		{ &subprog_flash_erase,				"flash_erase"		},
		#endif
		
		/* SCRIPT */
		#ifdef TERMINAL_PROGRAM_SCRIPT_START
		{ &terminal_program_script_start,			"script_start"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_END
		{ &terminal_program_script_end,			"script_end"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_DELAY_MS
		{ &terminal_program_script_delay_ms,			"script_delay_ms"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_SHOW
		{ &terminal_program_script_show,			"script_show"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_UNDO
		{ &terminal_program_script_undo,			"script_undo"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_MOVE
		{ &terminal_program_script_move,			"script_move"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_ROTATE
		{ &terminal_program_script_rotate,			"script_rotate"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_RUN
		{ &terminal_program_script_run,			"script_run"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_ERASE
		{ &terminal_program_script_erase,			"script_erase"	    },
		#endif
		
		/* MOTOR DRIVER TEST */
		#ifdef TERMINAL_PROGRAM_MOTOR_INIT
		{ &terminal_program_motor_init,			"motor_init"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_MOTOR_USE
		{ &terminal_program_motor_use,			"motor_use"	    },
		#endif
		
		/* MOTION */
		#ifdef TERMINAL_PROGRAM_MOVE
		{ &terminal_program_move,			"move"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_ROTATE
		{ &terminal_program_rotate,			"rotate"	    },
		#endif
		
		/* Last member of subprogram array must be null! */
		{ 0,									0}
		
	   };
