/*
 * storage_config.h
 *
 * Created: 5.12.2011 11:21:53
 *  Author: savpek
 */ 


#ifndef STORAGE_CONFIG_H_
#define STORAGE_CONFIG_H_

/* Memory segments.
 * Script segments -> */
#define STORAGE_ID_SCRIPT_0			0
#define STORAGE_ID_SCRIPT_1			1
#define STORAGE_ID_SCRIPT_2			2
#define STORAGE_ID_SCRIPT_3			3
#define STORAGE_ID_SCRIPT_4			4
/* Help file (in terminal) -> */
#define STORAGE_ID_HELP				5
/* Author file (in terminal) -> */
#define STORAGE_ID_AUTHOR			6

/* Defines how large segments are, must be
 * multiple of 4000 Bytes */
#define STORAGE_SEGMENT_SIZE		4000*2

/* Defines size of memory. */
#define STORAGE_SIZE				0x03FFFF

#endif /* STORAGE_CONFIG_H_ */