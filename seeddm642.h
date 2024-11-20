/*
 *  Copyright 2003 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== evmdm642.h ========
 *
 *  This file contains EVMDM642 board initialization and FPGA register access
 *  functions.
 *
 *  EVMDM642 BSL Version 1.00 - 07/02/2003
 *
 */

#ifndef EVMDM642_
#define EVMDM642_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl.h>
#include <csl_i2c.h>
#include <csl_gpio.h>

/*
 *  Note:  Bit definitions for each register field
 *         needs to be supplied here for the CPLD
 *	       and other board periperals.
 */

/* Compatability definitions */
#define NULL                 0

/* CPLD address definitions */
#define SEEDDM642_CPLD_BASE       0x90080000
#define EVMDM642_FPGASDRAM_BASE  0x80000000

/* CPLD Register Indices */
#define SEEDDM642_IOOUT       0x10 //write only
#define SEEDDM642_IOINPUT     0x10 //read ony
#define EVMDM642_THRESHMSB    0x12
#define EVMDM642_ISR          0x13
#define EVMDM642_IER          0x14
#define EVMDM642_GPIODIR      0x15
#define EVMDM642_GPIOSTAT     0x16
#define EVMDM642_FLASHPAGE    0x18

/* Read an 8-bit value from a CPLD register */
Uint8 SEEDDM642_rget(Int16 regnum);

/* Write an 8-bit value to a CPLD register */
void SEEDDM642_rset(Int16 regnum, Uint8 regval);

/* Spin in a delay loop for delay iterations */
void SEEDDM642_wait(Uint32 delay);

/* Spin in a delay loop for delay microseconds */
void SEEDDM642_waitusec(Uint32 delay);

/* Send a configuration file to the FPGA */
void EVMDM642_loadFpga(Uint32 fpgaaddr);

#ifdef __cplusplus
}
#endif

#endif
