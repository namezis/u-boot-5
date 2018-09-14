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
#include <mach/jz4760.h>

int board_early_init_f(void)
{
	__cpm_start_uart1();
	__gpio_as_uart1();
	return 0;
}

/* U-Boot common routines */
int checkboard(void)
{
	printf("Board: RETRO-GAME-V2.0 (retromini-rs97)\n");
	return 0;
}
