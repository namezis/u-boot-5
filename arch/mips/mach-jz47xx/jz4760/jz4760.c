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

#define EXTAL	12000000

#ifdef CONFIG_SPL_BUILD
/* Pointer to the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;
gd_t gdata __attribute__ ((section(".bss")));

void board_init_f(ulong dummy)
{
	typedef void __noreturn (*image_entry_noargs_t)(void);
	struct mmc *mmc;
	unsigned long count;
	struct image_header *header;
	int ret;

	debug_init();
	debug_print("\n\n\n--------------------------------------------------------------------------------\n");
	debug_print("SPL board_init_f\n");

	/* Set global data pointer */
	gd = &gdata;

	//timer_init();
	debug_print("PLL: ");
	ret = pll_init();
	if(ret == 0) {
		debug_print(" ok\n");
	}else{
		debug_print(" fail\n");
		hang();
	}

	debug_print("SDRAM: ");
	ret = sdram_init();
	if(ret == 0) {
		debug_print(" ok\n");
	}else{
		debug_print(" fail\n");
		hang();
	}

	debug_print("Cache: ");
	enable_caches();
	debug_print(" ok\n");

	/* Clear the BSS */
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);

	gd->flags |= GD_FLG_SPL_INIT;

	debug_print("mmc_initialize ");

	__cpm_select_msc_clk(0, 1);
	__cpm_start_msc0();

	ret = mmc_initialize(NULL);
	if (ret)
		hang();

	debug_print("find_mmc_device ");
	mmc = find_mmc_device(BOOT_DEVICE_MMC1);
	if (ret)
		hang();

	debug_print("mmc_init ");
	ret = mmc_init(mmc);
	if (ret)
		hang();
	debug_print("ok\n");

	header = (struct image_header *)(CONFIG_SYS_TEXT_BASE -
					 sizeof(struct image_header));

	debug_print("read header...");
	count = blk_dread(mmc_get_blk_desc(mmc),
		CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR,
		0x800, header);
	if (count == 0)
		hang();
	debug_print("ok\n");

	image_entry_noargs_t image_entry =
		(image_entry_noargs_t)CONFIG_SYS_TEXT_BASE;

	debug_print("calling u-boot\n");
	image_entry();

	hang();
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
