/*
 * mcu.h
 * Savpek Electronics
 *
 * Micorcontroller basic settings library.
 */

#ifndef MCU_H_
#define MCU_H_

#define MCU_CPU_F 	48000000UL
#define MCU_PBA_F	48000000UL
#define MCU_PBB_F	48000000UL

/* This must be bigger than any other clocks, DFLL loop
 * is used to generate rest of frequencies. */
#define MCU_DFLL_F	48000000UL

extern void mcu_init_clocks(void);

#endif /* MCU_H_ */
