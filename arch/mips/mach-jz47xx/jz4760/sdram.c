/*
 * JZ4760 memory configuration
 *
 * Copyright (c) 2018 Legitimate Data Company
 * Author: Jeremy Grosser <jeremy@synack.me>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <common.h>
#include <config.h>
#include <mach/jz4760.h>
#include <mach/jz4760_dram.h>

static volatile uint32_t NS_PER_TICK;

uint8_t ns_to_tck(uint8_t ns) {
	if((ns % NS_PER_TICK) == 0) {
		return ns / NS_PER_TICK;
	}else{
		return (ns / NS_PER_TICK) + 1;
	}
}

uint8_t ddr_longval(uint8_t v) {
	return (v / 2);
}

int sdram_init(void)
{
	/*
	 * 5.3.6	DDRC and DDR2 Memory Initialization Sequence
	 *
	 * 1 After system reset, wait system clock stable before initialize ddrc.
	 * 2 Configure the Clock-Control module for ddrc clocks.
	 * 		pll_init() should have already run, so this is done.
	 *
	 * 3 DDR Memory device need at least 200us initialization time after power-on before it can
	 * accept any command.
	 */

	REG_DDRC_CTRL = DDRC_CTRL_RESET;
	//udelay(200);

	NS_PER_TICK = (1000000000 / __cpm_get_mclk());

	/* INIT DDRC */
	/* 4 Configure DCFG */
	REG_DDRC_CFG = 0;

	/* 5 Configure DTIMING1 */
	REG_DDRC_TIMING1 = (
		(ddr_longval(ns_to_tck(DDR_tRAS))	<< DDRC_TIMING1_TRAS_BIT) |
		((ns_to_tck(DDR_tRTP) - 1)			<< DDRC_TIMING1_TRTP_BIT) |
		((ns_to_tck(DDR_tRP) - 1)			<< DDRC_TIMING1_TRP_BIT) |
		((ns_to_tck(DDR_tRCD) - 1)			<< DDRC_TIMING1_TRCD_BIT) |
		(ddr_longval(ns_to_tck(DDR_tRC))	<< DDRC_TIMING1_TRC_BIT) |
		((ns_to_tck(DDR_tRRD) - 1)			<< DDRC_TIMING1_TRRD_BIT) |
		((ns_to_tck(DDR_tWR) - 1)			<< DDRC_TIMING1_TWR_BIT) |
		((ns_to_tck(DDR_tWTR) - 1)			<< DDRC_TIMING1_TWTR_BIT)
		);
	
	/* 6 Configure DTIMING2 */
	REG_DDRC_TIMING2 = (
		(ddr_longval(ns_to_tck(DDR_tRFC))	<< DDRC_TIMING2_TRFC_BIT) |
		(ns_to_tck(DDR_tMINSR)				<< DDRC_TIMING2_TMINSR_BIT) |
		(ns_to_tck(DDR_tXP)					<< DDRC_TIMING2_TXP_BIT) |
		(ns_to_tck(DDR_tMRD)				<< DDRC_TIMING2_TMRD_BIT) |
		(ns_to_tck(DDR_tCKE)				<< DDRC_TIMING2_TCKE_BIT)
		);

	/* 7 Configure DMMAP0 */
	//REG_DDRC_MMAP0;

	/* 8 Configure DMMAP1 */
	//REG_DDRC_MMAP1;

	/* INIT DDR memory device */
	/* 9 Set CKE Pin HIGH : Configure DCTRL = 0x00000002. */

	/* 10 PRECHAREG-ALL : Configure DCTRL = 0x. */
	/* 11 Load-Mode-Register EMR2 : Configure DCTRL = 0x. */
	/* 12 Load-Mode-Register EMR3 : Configure DCTRL = 0x. */
	/* 13 Load-Mode-Register EMR1 : Configure DCTRL = 0x.  */
	/* 14 Load-Mode-Register MR with DLL reset : Configure DCTRL = 0x. */
	/* 15 PRECHAREG-ALL : Configure DCTRL = 0x. */
	/* 16 AUTO-REFRESH : Configure DCTRL = 0x. */
	/* 17 AUTO-REFRESH : Configure DCTRL = 0x. */
	/* 18 Load-Mode-Register MR with DLL reset end : Configure DCTRL = 0x. */
	/* 19 Load-Mode-Register EMR1 with OCD default : Configure DCTRL = 0x. */
	/* 20 Load-Mode-Register EMR1 with OCD exit : Configure DCTRL = 0x. */
	/* 21 Wait at least 200 tCK before next step. */

	/* Enable Refresh Counter */
	/* 22 Enable Refresh Counter : Configure DREFCNT = 0x. */
	REG_DDRC_REFCNT = (
		((DDR_tREFI / NS_PER_TICK / (16 * 1 << DDR_CLK_DIV)) << DDRC_REFCNT_CON_BIT) |
		(DDR_CLK_DIV << DDRC_REFCNT_CLKDIV_BIT) |
		DDRC_REFCNT_REF_EN
		);
	
	/* 23 AUTO-REFRESH : Configure DCTRL = 0x. */

	/* DQS Delay Detect */
	/* 24 Configure DDQSADJ = 0x. */
	/* 25 Configure DDQS = 0x. */
	REG_DDRC_DQS = DDRC_DQS_AUTO | DDRC_DQS_DET;

	/* 26 Read register DDQS. */
	/* 27 configure TSEL form min to max, under each value of TSEL, do 28. */
	/* 28 Configure {MSEL, HL, QUAR} register, form min delay to max delay (relate to 1.2.7). You
	 * need write/read some data by CPU or DMA or other device, to check if the sdram work
	 * properly. During this process, record the pass configure, you may found there has several
	 * configure pass the test, then chose the one that TSEL min &
	 * {MSEL, HL, QUAR} min passed <= {MSEL,HL,QUAR} <= {MSEL, HL, QUAR}max passed.
	 */
	
	/* END INITIALIZING SEQUENCE */

	return 1;
}
