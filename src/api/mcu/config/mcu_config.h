/*
 * mcu_config.h
 *
 * Created: 21.11.2011 11:43:53
 *  Author: savpek
 */ 


#ifndef MCU_CONFIG_H_
#define MCU_CONFIG_H_

#define MCU_CPU_F 	48000000UL
#define MCU_PBA_F	48000000UL
#define MCU_PBB_F	48000000UL

/* This must be bigger than any other clocks, DFLL loop
 * is used to generate rest of frequencies. */
#define MCU_DFLL_F	48000000UL

#endif /* MCU_CONFIG_H_ */