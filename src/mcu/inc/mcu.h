/*
 * mcu.h
 * Savpek Electronics
 *
 * Micorcontroller basic settings library.
 */

#ifndef MCU_H_
#define MCU_H_

/* ASF */
#include "compiler.h"

/* Internal */
#include "./mcu/config/mcu_config.h"

/* This inits clocks, see "./mcu/config/mcu_config.h" for settings */
extern void mcu_init_clocks(void);

#endif /* MCU_H_ */
