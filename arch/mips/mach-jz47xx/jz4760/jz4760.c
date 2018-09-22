/*
 * JZ4760 common routines
 *
 * Copyright (c) 2018 Legitimate Data Company
 * Author: Jeremy Grosser <jeremy@synack.me>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <mach/jz4760.h>
#include <mach/jz4760_dram.h>
#include <mmc.h>
#include <spl.h>
#include <serial.h>
#include <debug_uart.h>

#define EXTAL	12000000

#ifdef CONFIG_SPL_BUILD
/* Pointer to the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;
gd_t gdata __attribute__ ((section(".bss")));

void board_init_f(ulong dummy)
{
	//struct mmc *mmc;
	int ret;

	/* Set global data pointer */
	gd = &gdata;

	debug_uart_init();
	printascii("\n\n\n--------------------------------------------------------------------------------\n");
	printascii("SPL board_init_f\n");

	printascii("PLL: ");
	ret = pll_init();
	if(ret == 0) {
		printascii(" ok\n");
	}else{
		printascii(" fail\n");
		hang();
	}

	printascii("Timer: ");
	ret = timer_init();
	udelay(1);
	if(ret == 0) {
		printascii("ok\n");
	}else{
		printascii(" fail\n");
		hang();
	}

	printascii("SDRAM: ");
	ret = sdram_init();
	if(ret == 0) {
		printascii(" ok\n");
	}else{
		printascii(" fail\n");
		hang();
	}

	printascii("Cache: ");
	enable_caches();
	printascii(" ok\n");

	/* Clear the BSS */
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);

	gd->flags |= GD_FLG_SPL_INIT;

	/*
	printascii("MMC: ");
	ret = mmc_initialize(NULL);
	if (ret) {
		printascii("initialize fail\n");
		hang();
	}

	mmc = find_mmc_device(BOOT_DEVICE_MMC1);
	if(!mmc) {
		printascii("not found fail\n");
		hang();
	}

	ret = mmc_init(mmc);
	if (ret) {
		printascii("init fail\n");
		hang();
	}
	printascii("ok\n");
	*/

	//printascii("Console: ");
	//preloader_console_init();
	//printf("ok\n");
}
#endif /* CONFIG_SPL_BUILD */

int print_cpuinfo(void) {
	printf("Ingenic JZ4760\n");
	return 0;
}

int dram_init(void) {
	// Does nothing because DRAM was already configured in SPL
	return 0;
}

int board_mmc_init(bd_t *bd) {
	__gpio_as_msc0();
	__cpm_select_msc_clk(0, 1);
	__cpm_start_msc0();
	return jz_mmc_init((void __iomem *)MSC0_BASE);
}
