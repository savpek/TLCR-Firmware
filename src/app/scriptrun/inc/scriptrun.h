/*
 * script.h
 *
 * Created: 11.12.2011 16:46:40
 *  Author: savpek
 */ 


#ifndef SCRIPT_H_
#define SCRIPT_H_

#include "compiler.h"
#include "./errorh/inc/errorh.h"

#include "./api/scriptapi/inc/scriptapi.h"

/* Thread which runs all applications */
extern void scriptrun_thread(void *args);

/* This defines acces level for each app.
 * Lower number can be overrunned by higher level app. */
typedef enum
	{
	SCRIPTRUN_STOPPED = 0,
	SCRIPTRUN_UI = 1,
	SCRIPTRUN_TERMINAL = 2,
	} scriptrun_access_t;

typedef struct 
	{
	/* ID of script currently running */
	uint8_t current_script_id;
	
	/* Script line that is currently running */
	uint32_t current_script_line;
	
	/* Current script command (running atm) */
	scriptapi_cmd_t current_cmd;
	int32_t current_cmd_value;

	/* Is script running, and if it is, which
	 * app controls it */	
	scriptrun_access_t script_status;
	} scriptrun_t;

/* !! Control functions !! */

/* This function try take control over script app.
 * There are certain hierarchy defined that which
 * module can control script over others. See defination
 * of enum scriptrun_access_t */
extern errorc_t try_run_script(uint32_t script_id, scriptrun_access_t controller_app, uint32_t script_begin_line);
extern errorc_t try_stop_script(scriptrun_access_t controller_app);
#endif /* SCRIPT_H_ */