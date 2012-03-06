/*
 * spy_ioapi.c
 *
 * Created: 2.3.2012 14:35:48
 *  Author: savpek
 */ 

#include "compiler.h"
#include "./ioapi/public/ioapi.h"
#include "./ioapi/spy/spy_ioapi.h"

ioapi_spy_t spy_ioapi_tracker[TRACKER_SIZE];
uint8_t tracker_idx = 0;

static void clear_spy_tracker() {
	for(int i = 0; i < TRACKER_SIZE; i++) {
		(spy_ioapi_tracker[i]).pin_number = 0;
		(spy_ioapi_tracker[i]).cmd = 0;	
	}
}

void spy_ioapi_reset_tracker() {
	tracker_idx = 0;
	clear_spy_tracker();
}

void spy_ioapi_get_from_tracker(uint8_t tracker_idx) {
	
}

void spy_ioapi_output_high( uint8_t pin_number ) {
	(spy_ioapi_tracker[tracker_idx]).pin_number = pin_number;
	(spy_ioapi_tracker[tracker_idx]).cmd = IOAPI_OUT_HIGH;
	
	tracker_idx++;
}
void (*spy_ioapi_output_high_safe)(void) = NULL;

