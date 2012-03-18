#ifndef SPY_IOAPI_H_
#define SPY_IOAPI_H_

#include "compiler.h"

#define TRACKER_SIZE 30

typedef struct {
	uint32_t pin;
	uint32_t cmd;
} ioapi_spy_t;

extern ioapi_spy_t spy_ioapi_tracker[];

extern void spy_ioapi_get_from_tracker(uint8_t tracker_idx);
extern void spy_ioapi_output_high( uint8_t pin_number );
extern void (*backup_ioapi_output_high)( uint8_t );
extern void spy_ioapi_output_low( uint8_t pin_number );
extern void (*backup_ioapi_output_low)( uint8_t );
extern void spy_ioapi_reset_tracker( void);
#endif /* SPY_IOAPI_H_ */