/*
 * JZ4760 DDR2 memory parameters
 *
 * Copyright (c) 2018 Legitimate Data Company
 * Author: Jeremy Grosser <jeremy@synack.me>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#ifndef __JZ4760_DRAM_H__
#define __JZ4760_DRAM_H__
#include "jz4760.h"
#include "DDR2_H5PS1G63EFR-Y5C.h"

/* Paramters common to all RAM devices used */

/* Chip Select */
/* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */
#define DDR_CS1EN	0
/* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */
#define DDR_CS0EN	1

/* ROW : 12 to 18 row address, 1G only 512MB */
#define DDR_ROW		13
/* COL :  8 to 14 column address */
#define DDR_COL		10
/* Banks each chip: 0-4bank, 1-8bank */
#define DDR_BANK8	1
/* 0 - 16-bit data width, 1 - 32-bit data width */
#define DDR_DW32	0

/* Refresh period: 64ms / 32768 = 1.95 us , 2 ^ 15 = 32768 */
#define DDR_tREFI	7800
/* Clock Divider */
#define DDR_CLK_DIV	1

/* DDR3 Burst length: 0 - 8 burst, 2 - 4 burst , 1 - 4 or 8 (on the fly) */
#define DDR_BL		8

/* CAS latency: 5 to 14, tCK */
#define DDR_CL		5
/* DDR3 only: CAS Write Latency, 5 to 8 */
#define DDR_tCWL	(DDR_CL - 1)

#endif	/* __JZ4760_DRAM_H__ */

