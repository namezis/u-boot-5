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

static void pll_init(void)
{
	unsigned int reg, i;
	static const uint8_t n2FR[13] = {
		0, 0, 1, 2, 3, 0, 4, 0, 5, 0, 0, 0, 6,
	};

	/* write REG_DDRC_CTRL 8 times to clear ddr fifo */
	for (i = 0; i < 8; i++)
		REG_DDRC_CTRL = 0;

	REG_CPM_CPCCR = ((CFG_CDIV - 1) << CPM_CPCCR_CDIV_BIT) |
	  ((unsigned int) n2FR[CFG_H0DIV] << CPM_CPCCR_H0DIV_BIT) |
	  ((unsigned int) n2FR[CFG_H1DIV] << CPM_CPCCR_H1DIV_BIT) |
	  ((unsigned int) n2FR[CFG_H2DIV] << CPM_CPCCR_H2DIV_BIT) |
	  ((unsigned int) n2FR[CFG_PDIV]  << CPM_CPCCR_PDIV_BIT)  |
	  ((unsigned int) n2FR[CFG_C1DIV] << CPM_CPCCR_C1DIV_BIT) |
	  CPM_CPCCR_MEM | CPM_CPCCR_CE;

	reg = ((FEEDBACK(CFG_CPU_SPEED, CFG_EXTAL) - 1) << CPM_CPPCR_PLLM_BIT) |
	  ((INDIV(CFG_EXTAL) - 1) << CPM_CPPCR_PLLN_BIT) |
	  ((OUTDIV(CFG_CPU_SPEED) - 1) << CPM_CPPCR_PLLOD_BIT) |
	  CPM_CPPCR_PLLEN;
	if (BS(CFG_CPU_SPEED))
		reg |= CPM_CPPCR_PLL_BS_BIT;
	REG_CPM_CPPCR = reg;

	/* Wait for a stable output */
	while (!__cpm_pll_is_on());
	while (!(REG_CPM_CPPCR & CPM_CPPCR_PLLS));

	/* Disable pll0/2 divider */
	REG_CPM_CPCCR &= ~CPM_CPCCR_PCS;
}

/*
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
	//while(!(REG_CPM_CPPCR & CPM_CPPCR_PLLS));

	__cpm_set_cdiv(0);
	__cpm_set_hdiv(1);
	__cpm_set_h2div(2);
	__cpm_set_pdiv(2);
	__cpm_set_mdiv(2);
	__cpm_set_sdiv(2);
	__cpm_exclk_direct();

	REG_CPM_CPCCR |= CPM_CPCCR_MEM;
}
*/

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
