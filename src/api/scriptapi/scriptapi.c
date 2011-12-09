/*
 * scriptapi.c
 *
 * Created: 6.12.2011 14:26:39
 *  Author: savpek
 */ 

#include "compiler.h"

#include "./api/errorh/inc/errorh.h"
#include "./api/storage/inc/storage.h"
#include "./api/scriptapi/inc/scriptapi.h"
#include "./api/scriptapi/config/scriptapi_config.h"

/* This function inits values to handle you use. MUST BE CALLED BEFORE YOU USE HANDLE! */
errorc_t scriptapi_init_handle(scriptapi_t *handle, uint32_t script_id)
	{
	/* Get segment id (physical location in storage memory) */
	switch (script_id)
		{
		case 0:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_0;
			break;
		case 1:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_1;
			break;
		case 2:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_2;
			break;
		case 3:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_3;
			break;
		case 4:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_4;
			break;
		case 5:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_5;
			break;
		case 6:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_6;
			break;
		case 7:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_7;
			break;
		case 8:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_8;
			break;
		case 9:
			handle->storage_segment_id = STORAGE_ID_SCRIPT_9;
			break;
		default:
			#ifdef SCRIPTAPI_DEBUG
			errorh_print("\n\rSCRIPT_ID_OUT_OF_RANGE\n\r");
			#endif
			break;
		}
	
	/* Set script id you currently use. */	
	handle->script_id = script_id;
	
	/* Lets find last used index from current memory location, this can be done
	 * simply loop memory until we find 0xFFF... command. Slow, but working method. */
	handle->last_idx = 0;
	int32_t cmd_temp = 0;
	
	while(0xFFFFFFFF != cmd_temp)
		{
		/* If meet end of memory, dont go any further. */
		if(!storage_read_int32_t(handle->storage_segment_id, handle->last_idx*2, &cmd_temp))
			{
			#ifdef SCRIPTAPI_DEBUG
//			ERRORH_PRINT("");
			#endif
			return EC_OUT_OF_RANGE;	
			}
		handle->last_idx++;
		}
	
	/* Points to last idx which has data, so subtract one. */
	handle->last_idx -= 1;
	}

/* Get defined command from memory. Returns EC_FAILURE, EC_SUCCESS, EC_EMPTY, EC_EMPTY_CMD*/
extern errorc_t scriptapi_get_cmd(scriptapi_t *handle, uint32_t cmd_idx, scriptapi_cmd_t *ret_cmd, int32_t *ret_cmd_value)
	{
	/* Get command from flash memory */
	if(!storage_read_int32_t(handle->storage_segment_id, cmd_idx*2, (int32_t*)ret_cmd))
		{
		#ifdef SCRIPTAPI_DEBUG
//		ERRORH_PRINT("READ_CMD_ERROR");
		#endif
		return EC_FAILURE;
		}
			
	/* Get value */
	if(!storage_read_int32_t(handle->storage_segment_id, cmd_idx*2+1, (int32_t*)ret_cmd_value)) 
		{
		#ifdef SCRIPTAPI_DEBUG
//		ERRORH_PRINT("READ_VALUE_ERROR");
		#endif
		return EC_FAILURE;
		}
		
	/* If return value was 0xFFF ... which means that area was unwritten. */
	if(*ret_cmd == 0xFFFFFFFF)
		{
		#ifdef SCRIPTAPI_DEBUG
//		ERRORH_PRINT("CMD_EMPTY");
		#endif
		return EC_EMPTY;
		}
		
	if(*ret_cmd == 0)
		{
		/* This isn't actual error, it just informs that current
		 * memory had cleared cmd (command value 0) in it. */
		return EC_EMPTY_CMD;
		}

	return EC_SUCCESS;
	}

/* Puts command in defined index. */
extern errorc_t scriptapi_put_cmd(scriptapi_t *handle, scriptapi_cmd_t cmd, int32_t cmd_value)
	{
	/* Write command to memory */
	if(EC_SUCCESS != storage_write_int32_t(handle->storage_segment_id, (handle->last_idx)*2, cmd))
		{
		return EC_FAILURE;
		}
	
	/* Write value to memory */	
	if(EC_SUCCESS != storage_write_int32_t(handle->storage_segment_id, (handle->last_idx)*2+1, cmd_value))
		{
		return EC_FAILURE;
		}
	
	handle->last_idx++;
	
	return EC_SUCCESS;
	}

/* Writes "empty" command to defined command index */
extern errorc_t scriptapi_clr_cmd(scriptapi_t *handle, uint32_t cmd_idx)
	{
	/* This simply sets command value to 0 (all bits to 0). 
	 * So can overwrite any old value. */
	if(EC_SUCCESS != storage_write_int32_t(handle->storage_segment_id, cmd_idx*2, 0x00000000))
		{
		return EC_FAILURE;
		}
	return EC_SUCCESS;
	}

uint32_t scriptapi_get_last_idx(scriptapi_t *handle)
	{
	return handle->last_idx;
	}

/******************************************************************/
#if 0
static errorc_t s_get_segment(uint32_t script_id, uint32_t *segment_id)
	{
	switch (script_id)
		{
		case 0:
			*segment_id = STORAGE_ID_SCRIPT_0;
			return EC_SUCCESS;
			break;
		case 1:
			*segment_id = STORAGE_ID_SCRIPT_1;
			return EC_SUCCESS;
			break;
		case 2:
			*segment_id = STORAGE_ID_SCRIPT_2;
			return EC_SUCCESS;
			break;
		case 3:
			*segment_id = STORAGE_ID_SCRIPT_3;
			return EC_SUCCESS;
			break;
		case 4:
			*segment_id = STORAGE_ID_SCRIPT_4;
			return EC_SUCCESS;
			break;
		}
	
	return EC_SCRIPTAPI_INVALID_ID;
	}

/* This finds next non' empty and non' used index */
static errorc_t s_get_next_idx(uint32_t segment_id, uint32_t *cmd_idx)
	{
	scriptapi_t cmd_temp;
	int32_t cmd_value_temp = 0;
	
	/* Loop until find next empty index */
	while(cmd_temp != 0xFFFFFFFF)
		{
		/* If we meet end of segment before first empty location */
		if(EC_STORAGE_END == storage_read_int32_t(segment_id, *cmd_idx*2, (int32_t)&cmd_temp))
			{
			return EC_FAILURE;	
			}
		*cmd_idx++
		}
	
	return EC_SUCCESS;
	}

/* Get defined command from memory. Returns EC_SCRIPTAPI_NO_DATA if command has 0xFFFFFFF value. */
errorc_t scriptapi_get_cmd(uint32_t program_id, uint32_t cmd_idx, scriptapi_t *ret_cmd, int32_t *ret_cmd_value)
	{
	uint32_t segment_id = 0;
	
	if(s_get_segment(uint32_t program_id, &segment_id) != EC_SUCCESS)
		{
		return EC_SCRIPTAPI_INVALID_ID;	
		}

	/* Get command */ 
	if(!storage_read_int32_t(segment_id, cmd_idx*2, ret_cmd)) return EC_FAILURE;
	
	/* Get value */
	if(!storage_read_int32_t(segment_id, cmd_idx*2+1, ret_cmd_value)) return EC_FAILURE;
	
	return EC_SUCCESS;
	}

/* Puts command in next free position at flash memory */
errorc_t scriptapi_put_cmd(uint32_t script_id, scriptapi_t cmd, int32_t cmd_value)
	{
	uint32_t script_id = 0;
	
	if(s_get_segment(uint32_t script_id, &segment_id) != EC_SUCCESS)
		{
		return EC_SCRIPTAPI_INVALID_ID;	
		}

	/* Get next 0xFFF... address from flash memory. */
	if(!s_get_next_idx()) return EC_FAILURE;

	/* Writes command to firs cell. */
	storage_write_int32_t(segment_id, *cmd_idx*2, (int32_t)cmd);
	
	/* And value to second. */
	storage_write_int32_t(segment_id, *cmd_idx*2+1, (int32_t)cmd_value);
	
	return EC_SUCCESS;
	}

/* Writes "empty" command to defined command index */
errorc_t scriptapi_clr_cmd(uint32_t program_id, uint32_t cmd_idx)
	{
	uint32_t segment_id = 0;
	
	if(s_get_segment(uint32_t program_id, &segment_id) != EC_SUCCESS)
		{
		return EC_SCRIPTAPI_INVALID_ID;	
		}
		
	/* Write EMPTY command to memory. */
	storage_write_int32_t(s_get_segment(program_id), cmd_idx*2, 0x00000000);
	
	return EC_SUCCESS;
	}

#endif