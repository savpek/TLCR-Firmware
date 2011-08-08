/*
 * storage.h
 *
 * Created: 31.7.2011 18:19:10
 *  Author: Savpek Electronics
 *
 * Memory is divided in 8kB segments. Each segment
 * is read by ID. ID is actyally just number which tells where segment
 * starts. For example: segment ID 2 starts in flash addres 8000*2=0x3E80.
 * Segment size is allways 8kB, no matter how large or small it actuyally is.
 */ 

#ifndef STORAGE_H_
#define STORAGE_H_

#include "error_codes.h"

/*@Inits SPI transmission between MCU and FLASH chip */
extern void storage_init(void);

/*@Reads one uint32_t variable from memory. You need to know
 * segment and variable ID to read it from memory. Returns value
 * of variable from function.*/
extern uint32_t storage_get_variable(uint16_t segment_id, uint16_t variable_id);

/*@Put one variable to memory. You need to select segment and
 * variable ID to save it in memory. Returns false if write didn't done. */
extern uint8_t storage_put_variable(uint16_t segment_id, uint16_t variable_id);

/*@Writes single segment of data (8kB block) with Bytes.
 * Last parameter is write addr counter from beginning of segment.*/ 
extern errorc_t storage_write_segment(uint8_t segment_id, uint16_t next_byte_id, uint8_t data);
extern errorc_t storage_erase_segment(uint8_t segment_id);

/*@Reads single segment of data (8kB block), returns data as Bytes.
 * Last parameter is read addr counter from beginning of segment.
 * Remember end with storage_stop_read_segment()!!*/
extern errorc_t storage_read_segment(uint8_t segment_id, uint16_t next_byte_id, uint8_t* ret_data);

/*@Erase hole chip. */
extern uint8_t storage_erase_all(void);

/*@Gets flash ID and manufacturer ID from flash device.
 * This is for testing purposes. */
extern uint16_t storage_get_flash_id(void);

extern errorc_t storage_is_flash_ready(void);
#endif /* STORAGE_H_ */