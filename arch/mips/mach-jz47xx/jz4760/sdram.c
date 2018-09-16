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

#define TSEL_MIN 1
#define TSEL_MAX 4
#define MDELAY_MIN 1
#define MDELAY_MAX 16

static volatile uint32_t NS_PER_TICK;

int memtest(uint32_t *base, size_t len) {
	size_t i;

	len = len / sizeof(uint32_t);
	for(i = 0; i < len; i++) {
		base[i] = 0x33333333;
	}
	for(i = 0; i < len; i++) {
		if(base[i] != 0x33333333) {
			return 1;
		}
	}
	debug_print("memtest:PASS");
	return 0;
}

uint32_t sdram_size(int cs)
{
    uint32_t dw = DDR_DW32 ? 4 : 2;
    uint32_t banks = DDR_BANK8 ? 8 : 4;
    uint32_t size = 0;

    if ((cs == 0) && DDR_CS0EN) {
        size = (1 << (DDR_ROW + DDR_COL)) * dw * banks;
        if (DDR_CS1EN && (size > 0x20000000)) {
            size = 0x20000000;
		}
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
	uint32_t memsize, mem_base0, mem_mask0, mem_base1, mem_mask1;
	uint8_t mdelay, msel, tsel, hl, quar;
	int err;

	NS_PER_TICK = (1000000000 / __cpm_get_mclk());

	__cpm_start_ddr();

	REG_DDRC_CTRL = DDRC_CTRL_RESET;
	wait_ddr_ticks(DDR_tRFC);

	/* INIT DDRC */
	/* 4 Configure DCFG */
	debug_print("4 ");
	REG_DDRC_CFG = (
		DDRC_CFG_TYPE_DDR2 | DDRC_CFG_MPRT | 
		((DDR_ROW - 12) << DDRC_CFG_ROW_BIT) |
		((DDR_COL - 8) << DDRC_CFG_COL_BIT) |
		DDR_CS1EN | DDR_CS0EN |
		DDRC_CFG_CL_5 |
		(DDR_BANK8 << DDRC_CFG_BA) |
		DDRC_CFG_DW
		);

	/* 5 Configure DTIMING1 */
	debug_print("5 ");
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
	debug_print("6 ");
	REG_DDRC_TIMING2 = (
		(ddr_longval(ns_to_tck(DDR_tRFC))	<< DDRC_TIMING2_TRFC_BIT) |
		(ns_to_tck(DDR_tMINSR)				<< DDRC_TIMING2_TMINSR_BIT) |
		(ns_to_tck(DDR_tXP)					<< DDRC_TIMING2_TXP_BIT) |
		(ns_to_tck(DDR_tMRD)				<< DDRC_TIMING2_TMRD_BIT) |
		(ns_to_tck(DDR_tCKE)				<< DDRC_TIMING2_TCKE_BIT)
		);

	/* 7 Configure DMMAP0 */
	debug_print("7 ");
	memsize = sdram_size(0);
	mem_base0 = DDR_MEM_PHY_BASE >> 24;
	mem_mask0 = (0xff & ~((((memsize / (DDR_CS1EN + DDR_CS0EN)) >> 24) - 1) & DDRC_MMAP_MASK_MASK));
	REG_DDRC_MMAP0 = ((mem_base0 << DDRC_MMAP_BASE_BIT) | mem_mask0);

	/* 8 Configure DMMAP1 */
	debug_print("8 ");
	mem_base1 = (DDR_MEM_PHY_BASE + (memsize / (DDR_CS1EN + DDR_CS0EN))) >> 24;
	mem_mask1 = mem_mask0;
	REG_DDRC_MMAP1 = ((mem_base1 << DDRC_MMAP_BASE_BIT) | mem_mask1);

	/* INIT DDR memory device */
	/* 9 Set CKE Pin HIGH : Configure DCTRL = 0x00000002. */
	debug_print("9 ");
	REG_DDRC_CTRL = DDRC_CTRL_CKE;
	wait_ddr_ticks(DDR_tRP);

	/* 10 PRECHARGE-ALL : */
	debug_print("10 ");
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_PREC | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRP);

	/* 11 Load-Mode-Register EMR2 : Configure DCTRL = 0x. */
	debug_print("11 ");
	REG_DDRC_LMR = (DDRC_LMR_BA_EMRS2 | DDRC_LMR_CMD_LMR | DDRC_LMR_START);

	/* 12 Load-Mode-Register EMR3 : Configure DCTRL = 0x. */
	debug_print("12 ");
	REG_DDRC_LMR = (DDRC_LMR_BA_EMRS3 | DDRC_LMR_CMD_LMR | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tMRD);

	/* 13 Load-Mode-Register EMR1 : Configure DCTRL = 0x.  */
	debug_print("13 ");
	// TODO: not sure why we set DQS_DIS here
	/*
	REG_DDRC_LMR = (
			(DDR_EMRS1_DQS_DIS << DDRC_LMR_DDR_ADDR_BIT) | 
			(DDRC_LMR_BA_EMRS1 | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);
			*/
	REG_DDRC_LMR = (DDRC_LMR_BA_EMRS1 | DDRC_LMR_CMD_LMR | DDRC_LMR_START);

	wait_ddr_ticks(DDR_tRP);

	/* 14 Load-Mode-Register MR with DLL reset : Configure DCTRL = 0x. */
	// TODO: no idea about the TWR DLL_RST here either
	debug_print("14 ");
	REG_DDRC_LMR = (
			(((REG_DDRC_TIMING1 & DDRC_TIMING1_TWR_MASK) >> DDRC_TIMING1_TWR_BIT) << DDR2_MRS_DLL_RST) |
			(DDRC_LMR_BA_MRS | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);

	wait_ddr_ticks(DDR_tMRD);

	/* 15 PRECHARGE-ALL : Configure DCTRL = 0x. */
	debug_print("15 ");
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_PREC | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRP);

	/* 16 AUTO-REFRESH : Configure DCTRL = 0x. */
	debug_print("16 ");
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_AUREF | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRFC);

	/* 17 AUTO-REFRESH : Configure DCTRL = 0x. */
	debug_print("17 ");
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_AUREF | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRFC);

	/* 18 Load-Mode-Register MR with DLL reset end : Configure DCTRL = 0x. */
	debug_print("18 ");
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
	debug_print("19 ");
	REG_DDRC_LMR = (
			((DDR_EMRS1_DQS_DIS | DDR_EMRS1_OCD_DFLT) << DDRC_LMR_DDR_ADDR_BIT) |
			(DDRC_LMR_BA_EMRS1 | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);

	/* 20 Load-Mode-Register EMR1 with OCD exit : Configure DCTRL = 0x. */
	debug_print("20 ");
	REG_DDRC_LMR = (
			(DDR_EMRS1_DQS_DIS << DDRC_LMR_DDR_ADDR_BIT) |
			(DDRC_LMR_BA_EMRS1 | DDRC_LMR_CMD_LMR | DDRC_LMR_START)
			);

	/* 21 Wait at least 200 tCK before next step. */
	debug_print("21 ");
	wait_ddr_ticks(DDR_tRFC);

	/* Enable Refresh Counter */
	/* 22 Enable Refresh Counter : Configure DREFCNT = 0x. */
	debug_print("22 ");
	REG_DDRC_REFCNT = (
		((DDR_tREFI / NS_PER_TICK / (16 * 1 << DDR_CLK_DIV)) << DDRC_REFCNT_CON_BIT) |
		(DDR_CLK_DIV << DDRC_REFCNT_CLKDIV_BIT) |
		DDRC_REFCNT_REF_EN
		);
	
	/* 23 AUTO-REFRESH : Configure DCTRL = 0x. */
	debug_print("23 ");
	REG_DDRC_LMR = (DDRC_LMR_BA_MRS | DDRC_LMR_CMD_AUREF | DDRC_LMR_START);
	wait_ddr_ticks(DDR_tRFC);

	/* DQS Delay Detect */
	/* 24 Configure DDQSADJ = 0x. */
	debug_print("24 ");
	// automatic delay adjust is disabled
	REG_DDRC_DQS_ADJ = 0;

	/* 25 Configure DDQS = 0x. */
	debug_print("25 ");
	REG_DDRC_DQS = (DDRC_DQS_AUTO | DDRC_DQS_DET | DDRC_DQS_SRDET);

	/* 26 Read register DDQS. */
	debug_print("26 ");
	/* wait for DQS READY=1 */
	while(!(REG_DDRC_DQS & DDRC_DQS_READY));

	/* 27 configure TSEL form min to max, under each value of TSEL, do 28. */
	debug_print("27 ");
	for(tsel = TSEL_MIN; tsel < TSEL_MAX; tsel++) {
		/* 28 Configure {MSEL, HL, QUAR} register, form min delay to max delay (relate to 1.2.7). You
		 * need write/read some data by CPU or DMA or other device, to check if the sdram work
		 * properly. During this process, record the pass configure, you may found there has several
		 * configure pass the test, then chose the one that TSEL min &
		 * {MSEL, HL, QUAR} min passed <= {MSEL,HL,QUAR} <= {MSEL, HL, QUAR}max passed.
		 */
		for(mdelay = MDELAY_MIN; mdelay < MDELAY_MAX; mdelay++) {
			msel = mdelay / 4;
			hl = ((mdelay / 2) & 1) ^ 1;
			quar = mdelay & 1;

			REG_DDRC_DELAYCTRL1 = (
				(tsel << DDRC_DELAYCTRL1_TSEL_BIT) |
				(msel << DDRC_DELAYCTRL1_MSEL_BIT) |
				(hl << DDRC_DELAYCTRL1_HL_BIT) |
				(quar << DDRC_DELAYCTRL1_QUAR_BIT)
				);
			wait_ddr_ticks(DDR_tRP);
			
			err = memtest((uint32_t *)(DDR_MEM_PHY_BASE), 0x1000);
			if(err == 0) {
				debug_print(" TSEL=");
				debug_printhex(tsel);
				debug_print(" MSEL=");
				debug_printhex(msel);
				debug_print(" HL=");
				debug_printhex(hl);
				debug_print(" QUAR=");
				debug_printhex(quar);
				debug_print("\n");
				goto sdram_init_end;
			}
		}
	}

sdram_init_end:
	/* END INITIALIZING SEQUENCE */
	return err;
}
