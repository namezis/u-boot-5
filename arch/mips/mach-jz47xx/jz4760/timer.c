/*
 * JZ4760 timer
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <mach/jz4760.h>

int timer_init(void)
{
	/* Stop counting up */
	REG_TCU_TECR |= TCU_TESR_OSTEN;

	/* Shut Down PWM output */
	REG_OST_CSR = OST_CSR_PWM_SD;

	/* Reset counter value */
	REG_OST_CNT = 0;

	/* 
	 * Overflow mode
	 * EXTAL as input
	 * x16 prescaler
	 */
	REG_OST_CSR = (OST_CSR_CNT_MD | OST_CSR_EXT_EN | OST_CSR_PRESCALE16);

	/* Enable the counter */
	REG_TCU_TESR |= TCU_TESR_OSTEN;

	return 0;
}

unsigned long get_timer(unsigned long base)
{
	return REG_OST_CNT;
}

void __udelay(unsigned long usec)
{
	/* OST count increments at EXTAL * 16 */
	u64 end = get_timer(0) + ((u64)usec * 16);
	while (get_timer(0) < end);
}

unsigned long long get_ticks(void)
{
	return (u64)get_timer(0);
}
