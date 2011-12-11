/*
 * list_of_subprograms.c
 *
 * Created: 24.8.2011 1:31:04
 *  Author: savpek
 */ 

#include "compiler.h"
#include "./app/terminal/config/terminal_program_list.h"
#include "./api/terminalapi/inc/terminalapi.h"

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
		{ &subprog_script_start,			"script_start"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_END
		{ &subprog_script_end,			"script_end"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_DELAY_MS
		{ &subprog_script_delay_ms,			"script_delay_ms"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_SHOW
		{ &subprog_script_show,			"script_show"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_UNDO
		{ &subprog_script_undo,			"script_undo"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_MOVE
		{ &subprog_script_move,			"script_move"	    },
		#endif
		#ifdef TERMINAL_PROGRAM_SCRIPT_ROTATE
		{ &subprog_script_rotate,			"script_rotate"	    },
		#endif
		
		/* Last member of subprogram array must be null! */
		{ 0,									0}
		
	   };
