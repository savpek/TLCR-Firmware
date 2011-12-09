/*
 * scriptapi.h
 *
 * Created: 6.12.2011 14:26:11
 *  Author: savpek
 */ 


#ifndef SCRIPTAPI_H_
#define SCRIPTAPI_H_

#include "compiler.h"
#include "./api/scriptapi/config/scriptapi_config.h"

/* These are command codes, saved in flashmemory. */
typedef enum {
	SCRIPTAPI_CMD_EMPTY = 0,
	SCRIPTAPI_CMD_DELAY = 1,
	SCRIPTAPI_CMD_MOVE = 2,
	SCRIPTAPI_CMD_ROTATE = 3
}scriptapi_cmd_t;

typedef struct {
	/* Current script id */
	uint32_t script_id;
	
	/* Last index in current memory segment. */
	uint32_t last_idx;
	
	/* This contains segment id of current script id,
	 * definations for segments can be found from storage api */
	uint32_t storage_segment_id;
}scriptapi_t;

/* This function inits values to handle you use. MUST BE CALLED BEFORE YOU USE HANDLE! */
extern errorc_t scriptapi_init_handle(scriptapi_t *handle, uint32_t script_id);

/* Get defined command from memory. Returns EC_NO_DATA, EC_SUCCESS, EC_OUT_OF_RANGE */
extern errorc_t scriptapi_get_cmd(scriptapi_t *handle, uint32_t cmd_idx, scriptapi_cmd_t *ret_cmd, int32_t *ret_cmd_value);

/* Puts command in defined index.
 * Returns: EC_SUCCESS, EC_NOT_EMPTY, EC_MEMORY_FULL */
extern errorc_t scriptapi_put_cmd(scriptapi_t *handle, scriptapi_cmd_t cmd, int32_t cmd_value);

/* Writes "empty" command to defined command index */
extern errorc_t scriptapi_clr_cmd(scriptapi_t *handle, uint32_t cmd_idx);

/* Gets last index of script */
extern uint32_t scriptapi_get_last_idx(scriptapi_t *handle);
	
#endif /* SCRIPTAPI_H_ */