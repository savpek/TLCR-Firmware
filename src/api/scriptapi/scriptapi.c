/*
 * scriptapi.c
 *
 * Created: 6.12.2011 14:26:39
 *  Author: savpek
 */

#include "compiler.h"

#include "./errorh/inc/errorh.h"
#include "./storage/inc/storage.h"
#include "./scriptapi/inc/scriptapi.h"
#include "./scriptapi/config/scriptapi_config.h"

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
		#endif
		return EC_FAILURE;
		}

	/* Get value */
	if(!storage_read_int32_t(handle->storage_segment_id, cmd_idx*2+1, (int32_t*)ret_cmd_value))
		{
		#ifdef SCRIPTAPI_DEBUG
		#endif
		return EC_FAILURE;
		}

	/* If return value was 0xFFF ... which means that area was unwritten. */
	if(*ret_cmd == 0xFFFFFFFF)
		{
		#ifdef SCRIPTAPI_DEBUG
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

