/*
 * RS97 setup code
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <serial.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <mach/jz4760.h>
//#include <mach/jz4760_dram.h>

#define JZ_GPIO(bank, pin)     ((32 * (bank)) + (pin))


int board_early_init_f(void)
{
	pll_init();
	__gpio_as_uart2();
	__cpm_start_uart2();

	__gpio_as_uart1();
	__cpm_start_uart1();

	__gpio_as_msc0();

	/* SYS_POWER_IND high (LED blue, VBUS on) */
	gpio_direction_output(JZ_GPIO(5, 15), 1);

	/* LEDs off */
	gpio_direction_output(JZ_GPIO(2, 0), 0);
	gpio_direction_output(JZ_GPIO(2, 1), 0);
	gpio_direction_output(JZ_GPIO(2, 2), 0);
	gpio_direction_output(JZ_GPIO(2, 3), 0);

	return 0;
}

int dram_init(void)
{
	return 0;
}

/* U-Boot common routines */
int checkboard(void)
{
	printf("Board: RETRO-GAME-V2.0 (retromini-rs97)\n");
	return 0;
}

#ifdef CONFIG_SPL_BUILD

#if defined(CONFIG_SPL_MMC_SUPPORT)
int board_mmc_init(bd_t *bd)
{
	__gpio_as_msc0();
	return jz_mmc_init((void __iomem *)MSC0_BASE);
}
#endif

//H5PS1G63EFR-Y5C
#endif
