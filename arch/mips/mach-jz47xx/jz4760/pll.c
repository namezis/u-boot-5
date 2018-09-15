/*
 * JZ4760 PLL configuration
 *
 * Copyright (c) 2018 Legitimate Data Company
 * Author: Jeremy Grosser <jeremy@synack.me>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <mach/jz4760.h>

uint32_t pll_calc(uint32_t pll_out) {
	// CLKOUT = EXTAL * (M / N) * (1 / (OD * 2))
	uint8_t no;
	uint8_t m, n, od;
	uint32_t plcr;

	if(pll_out > 1500) {
		// PLL cannot be > 1500
		debug_print("pll_out cannot be > 1500\n");
		return 0;
	}else if(pll_out >= 624) {
		od = 0;
		no = 1;
	}else if(pll_out >= 336) {
		od = 1;
		no = 2;
	}else if(pll_out >= 168) {
		od = 2;
		no = 4;
	}else if(pll_out >= 72) {
		od = 3;
		no = 8;
	}else{
		// PLL cannot be < 72
		debug_print("pll_out cannot be < 72\n");
		return 0;
	}

	n = 1;
	m = (((pll_out / (JZ4760_SYS_EXTAL / 1000000)) * no * n) >> 1);

	if(m > 127 || m < 2) {
		// Invalid PLL_M
		debug_print("pll_m out of range\n");
		return 0;
	}

	/*
	debug_print("M=0x");
	debug_printhex(m);
	debug_putc(' ');
	debug_print("N=0x");
	debug_printhex(n);
	debug_putc(' ');
	debug_print("OD=0x");
	debug_printhex(od);
	debug_putc('\n');
	*/

	plcr = (m << 24) | (n << 18) | (od << 16);

	return plcr;
}

int pll_init(void) {
	uint32_t cfcr;
	uint32_t pll;
	int n2FR[9] = {
		0, 0, 1, 2, 3, 0, 4, 0, 5,
	};

	int div[6] = {1, 2, 4, 4, 4, 4};

	cfcr = CPM_CPCCR_PCS |
		(n2FR[div[0]] << CPM_CPCCR_CDIV_BIT) |
		(n2FR[div[1]] << CPM_CPCCR_HDIV_BIT) |
		(n2FR[div[2]] << CPM_CPCCR_H2DIV_BIT) |
		(n2FR[div[3]] << CPM_CPCCR_PDIV_BIT) |
		(n2FR[div[4]] << CPM_CPCCR_MDIV_BIT) |
		(n2FR[div[5]] << CPM_CPCCR_SDIV_BIT);

	// write REG_DDRC_CTRL 8 times to clear ddr fifo
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;
	REG_DDRC_CTRL = 0;

	/* EXCLK divder is 1 */
	cfcr &= ~CPM_CPCCR_ECS;

	/* invert this if we're using MDDR */
	cfcr |= CPM_CPCCR_MEM;

	/* change enable */
	cfcr |= CPM_CPCCR_CE;

	/* PLL0 */
	pll = pll_calc(CONFIG_SYS_MHZ);
	if(pll == 0) {
		debug_print("PLL0: pll_calc failed\n");
		return 1;
	}
	REG_CPM_CPPCR = (
			pll |
			(0x20 << CPM_CPPCR_PLLST_BIT) |
			CPM_CPPCR_PLLEN);
	/* wait for pll0 output stable ...*/
	while (!(REG_CPM_CPPCR & CPM_CPPCR_PLLS));

	/* configure dividers */
	REG_CPM_CPCCR = cfcr;

	/* 48 MHz USB Host Clock */
	REG_CPM_UHCCDR = ((CONFIG_SYS_MHZ / 4) / 48) - 1;

	/* PLL1 */
	pll = pll_calc(CONFIG_SYS_MHZ / 4);
	if(pll == 0) {
		debug_print("PLL1: pll_calc failed\n");
		return 2;
	}
	REG_CPM_CPPCR1 = (
			pll |
			CPM_CPPCR1_PLL1EN);
	
	/* wait for pll1 output stable ...*/
	while(!(REG_CPM_CPPCR1 & CPM_CPPCR1_PLL1S));

	/*
	debug_print("pll_init complete:\n");
	debug_print("  PLL0 = ");
	debug_printhex4(__cpm_get_pllout());
	debug_print("  PLL1 = ");
	debug_printhex4(__cpm_get_pll1out());
	*/
	return 0;
}
