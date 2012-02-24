/*
 * clocks.h
 *
 * Created: 24.2.2012 0:14:58
 *  Author: savpek
 */ 


#ifndef CLOCKS_H_
#define CLOCKS_H_

#define INITD_CPU_F 	48000000UL
#define INITD_PBA_F		48000000UL
#define INITD_PBB_F		48000000UL

/* This must be bigger than any other clocks, DFLL loop
 * is used to generate rest of frequencies. */
#define INITD_DFLL_F	48000000UL

#endif /* CLOCKS_H_ */