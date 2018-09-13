/*
 * JZ4760 PLL setup
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <mach/jz4760.h>


void pll_init(void)
{
	__cpm_enable_pll_change();
	REG_CPM_CPPCR = (
			(48 << CPM_CPPCR_PLLM_BIT) |
			(1 << CPM_CPPCR_PLLN_BIT) |
			(0 << CPM_CPPCR_PLLOD_BIT) |
			(0x20 << CPM_CPPCR_PLLST_BIT) |
			CPM_CPPCR_PLLEN
	);
	// Wait for PLL stable
	while(!(REG_CPM_CPPCR & CPM_CPPCR_PLLS));

	__cpm_set_cdiv(0);
	__cpm_set_hdiv(1);
	__cpm_set_h2div(2);
	__cpm_set_pdiv(2);
	__cpm_set_mdiv(2);
	__cpm_set_sdiv(2);
	__cpm_exclk_direct();

	REG_CPM_CPCCR |= CPM_CPCCR_MEM;
}

void jz4760_clk_ungate_mmc(void)
{
	__cpm_select_msc_clk(0, 0);
	__cpm_select_msc_clk(1, 0);
	__cpm_select_msc_clk(2, 0);

	__cpm_start_msc0();
	__cpm_start_msc1();
	__cpm_start_msc2();
}

void jz4760_clk_ungate_uart(const unsigned int uart)
{
	switch(uart) {
		case 0: __cpm_start_uart0(); break;
		case 1: __cpm_start_uart1(); break;
		case 2: __cpm_start_uart2(); break;
		case 3: __cpm_start_uart3(); break;
		default:
			printf("%s[%i]: Invalid UART %d\n", __func__, __LINE__, uart);
	}
}
