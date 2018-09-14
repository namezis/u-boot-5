/*
 * JZ4760 common routines
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <mach/jz4760.h>
#include <mmc.h>
#include <spl.h>

#define EXTAL	12000000

#ifdef CONFIG_SPL_BUILD
/* Pointer to the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;
gd_t gdata __attribute__ ((section(".bss")));

uint32_t pll_calc(uint32_t pll_out) {
	// PLL M = (PLL_OUT/EXTAL * NO * N >> 1)
	// PLL N = 2
	// PLL OD = 1
	uint8_t no;
	uint8_t m, n, od;
	uint32_t plcr;

	if(pll_out > 1500) {
		// PLL cannot be > 1500
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
		return 0;
	}

	n = 2;
	m = (((pll_out / EXTAL) * no * n) >> 1);

	if(m > 127 || m < 2) {
		// Invalid PLL_M
		return 0;
	}

	plcr = (m << 24) | (n << 18) | (od << 16);

	return plcr;
}

void pll_init(void) {
	uint32_t cfcr;
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

	if(EXTAL > 16000000) {
		cfcr |= CPM_CPCCR_ECS;
	}else{
		cfcr &= ~CPM_CPCCR_ECS;
	}

	/* invert this if we're using MDDR */
	cfcr |= CPM_CPCCR_MEM;

	/* enable clocks */
	cfcr |= CPM_CPCCR_CE;

	/* PLL0 */
	REG_CPM_CPPCR = (
			pll_calc(CONFIG_SYS_MHZ * 1000000) |
			(0x20 << CPM_CPPCR_PLLST_BIT) |
			CPM_CPPCR_PLLEN);
	/* wait for pll0 output stable ...*/
	while (!(REG_CPM_CPPCR & CPM_CPPCR_PLLS));

	/* configure dividers */
	REG_CPM_CPCCR = cfcr;

	/* 48 MHz USB Host Clock */
	REG_CPM_UHCCDR = ((CONFIG_SYS_MHZ / 4) / 48) - 1;

	/* PLL1 */
	REG_CPM_CPPCR1 = (
			pll_calc((CONFIG_SYS_MHZ * 1000000) / 4) |
			CPM_CPPCR1_PLL1EN);
	
	__cpm_enable_pll_change();

	/* wait for pll1 output stable ...*/
	while(!(REG_CPM_CPPCR1 & CPM_CPPCR1_PLL1S));

}

void board_init_f(ulong dummy)
{
	//typedef void __noreturn (*image_entry_noargs_t)(void);
	//struct mmc *mmc;
	//unsigned long count;
	//struct image_header *header;
	//int ret;

	/* Set global data pointer */
	gd = &gdata;

	//timer_init();
	pll_init();
	//sdram_init();
	enable_caches();

	/* Clear the BSS */
	//memset(__bss_start, 0, (char *)&__bss_end - __bss_start);

	gd->flags |= GD_FLG_SPL_INIT;

	/*
	ret = mmc_initialize(NULL);
	if (ret)
		hang();

	mmc = find_mmc_device(BOOT_DEVICE_MMC1);
	if (ret)
		hang();

	ret = mmc_init(mmc);
	if (ret)
		hang();

	header = (struct image_header *)(CONFIG_SYS_TEXT_BASE -
					 sizeof(struct image_header));

	count = blk_dread(mmc_get_blk_desc(mmc),
		CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR,
		0x800, header);
	if (count == 0)
		hang();

	image_entry_noargs_t image_entry =
		(image_entry_noargs_t)CONFIG_SYS_TEXT_BASE;

	image_entry();
	*/

	//hang();
}
#endif /* CONFIG_SPL_BUILD */
