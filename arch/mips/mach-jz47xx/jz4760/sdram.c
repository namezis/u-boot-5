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

int memtest(uint32_t *base, size_t len) {
	debug_print("memtest start...");
	memset(base, 0x33333333, (len / sizeof(uint32_t)));
	while(len--) {
		if(base[len] != 0x33333333) {
			debug_print("fail!\n");
			return 1;
		}
	}
	debug_print("ok!\n");
	return 0;
}

uint32_t sdram_size(int cs)
{
    uint32_t dw = DDR_DW32 ? 4 : 2;
    uint32_t banks = DDR_BANK8 ? 8 : 4;
    uint32_t size = 0;

    if ((cs == 0) && DDR_CS0EN) {
        size = (1 << (DDR_ROW + DDR_COL)) * dw * banks;
        if (DDR_CS1EN && (size > 0x20000000))
            size = 0x20000000;                                                                                                                                                                                                                                
    } else if ((cs == 1) && DDR_CS1EN) {                                                                                                                                                                                                                      
        size = (1 << (DDR_ROW + DDR_COL)) * dw * banks;                                                                                                                                                                                                       
    }                                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                                              
    return size;                                                                                                                                                                                                                                              
}

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

void wait_ddr_ticks(uint32_t tck) {
	tck = (__cpm_get_cclk() / 1000000000);
	while(tck--);
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

	NS_PER_TICK = (1000000000 / __cpm_get_mclk());

	REG_DDRC_CTRL = DDRC_CTRL_RESET;
	wait_ddr_ticks(DDR_tRFC);

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
	REG_DDRC_MMAP0 = (DDRC_MMAP0_BASE | DDRC_MMAP_MASK_MASK);

	/* 8 Configure DMMAP1 */
	// TODO: figure out actual memory size before mapping
	REG_DDRC_MMAP1 = (DDRC_MMAP1_BASE_128M | DDRC_MMAP_MASK_MASK);

	/* INIT DDR memory device */
	/* 9 Set CKE Pin HIGH : Configure DCTRL = 0x00000002. */
	REG_DDRC_CTRL = DDRC_CTRL_CKE;
	wait_ddr_ticks(DDR_tRP);

	/* 10 PRECHARGE-ALL : */
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_PREC | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRP);

	/* 11 Load-Mode-Register EMR2 : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (DDRC_LMR_BA_EMRS2 | DDRC_LMR_CMD_LMR | DDRC_LMR_START);

	/* 12 Load-Mode-Register EMR3 : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (DDRC_LMR_BA_EMRS3 | DDRC_LMR_CMD_LMR | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tMRD);

	/* 13 Load-Mode-Register EMR1 : Configure DCTRL = 0x.  */
	// TODO: not sure why we set DQS_DIS here
	REG_DDRC_LMR = (
			(DDR_EMRS1_DQS_DIS << DDRC_LMR_DDR_ADDR_BIT) | 
			(DDRC_LMR_BA_EMRS1 | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);

	wait_ddr_ticks(DDR_tRP);

	/* 14 Load-Mode-Register MR with DLL reset : Configure DCTRL = 0x. */
	// TODO: no idea about the TWR DLL_RST here either
	REG_DDRC_LMR = (
			(((REG_DDRC_TIMING1 & DDRC_TIMING1_TWR_MASK) >> DDRC_TIMING1_TWR_BIT) << DDR2_MRS_DLL_RST) |
			(DDRC_LMR_BA_MRS | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);

	wait_ddr_ticks(DDR_tMRD);

	/* 15 PRECHARGE-ALL : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_PREC | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRP);

	/* 16 AUTO-REFRESH : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_AUREF | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRFC);

	/* 17 AUTO-REFRESH : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_AUREF | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRFC);

	/* 18 Load-Mode-Register MR with DLL reset end : Configure DCTRL = 0x. */
	// hell if I know.
	REG_DDRC_LMR = (
		(
			(
			 	(((REG_DDRC_TIMING1 & DDRC_TIMING1_TWR_MASK) >> DDRC_TIMING1_TWR_BIT) << DDR2_MRS_DLL_RST) |
				(DDR_CL << DDR_MRS_CAS_BIT) |
				(DDR_MRS_BL_4)
			) << DDRC_LMR_DDR_ADDR_BIT
		) | (
			DDRC_LMR_BA_MRS | DDRC_LMR_CMD_LMR | DDRC_LMR_START
		)
	);

	wait_ddr_ticks(DDR_tRFC);

	/* 19 Load-Mode-Register EMR1 with OCD default : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (
			((DDR_EMRS1_DQS_DIS | DDR_EMRS1_OCD_DFLT) << DDRC_LMR_DDR_ADDR_BIT) |
			(DDRC_LMR_BA_EMRS1 | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);

	/* 20 Load-Mode-Register EMR1 with OCD exit : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (
			(DDR_EMRS1_DQS_DIS << DDRC_LMR_DDR_ADDR_BIT) |
			(DDRC_LMR_BA_EMRS1 | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);

	/* 21 Wait at least 200 tCK before next step. */
	wait_ddr_ticks(DDR_tRFC);

	/* Enable Refresh Counter */
	/* 22 Enable Refresh Counter : Configure DREFCNT = 0x. */
	REG_DDRC_REFCNT = (
		((DDR_tREFI / NS_PER_TICK / (16 * 1 << DDR_CLK_DIV)) << DDRC_REFCNT_CON_BIT) |
		(DDR_CLK_DIV << DDRC_REFCNT_CLKDIV_BIT) |
		DDRC_REFCNT_REF_EN
		);
	
	/* 23 AUTO-REFRESH : Configure DCTRL = 0x. */
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_AUREF | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRFC);

	/* DQS Delay Detect */
	/* 24 Configure DDQSADJ = 0x. */
	// automatic delay adjust is disabled
	REG_DDRC_DQS_ADJ = 0;

	/* 25 Configure DDQS = 0x. */
	REG_DDRC_DQS = (DDRC_DQS_AUTO | DDRC_DQS_DET);

	/* 26 Read register DDQS. */
	while(!(REG_DDRC_DQS & (DDRC_DQS_ERROR | DDRC_DQS_READY)));

	/* 27 configure TSEL form min to max, under each value of TSEL, do 28. */

	/* 28 Configure {MSEL, HL, QUAR} register, form min delay to max delay (relate to 1.2.7). You
	 * need write/read some data by CPU or DMA or other device, to check if the sdram work
	 * properly. During this process, record the pass configure, you may found there has several
	 * configure pass the test, then chose the one that TSEL min &
	 * {MSEL, HL, QUAR} min passed <= {MSEL,HL,QUAR} <= {MSEL, HL, QUAR}max passed.
	 */
	
	/* END INITIALIZING SEQUENCE */

	return memtest((uint32_t *)(0x20000000 - 0x1000), 0x1000);
}
