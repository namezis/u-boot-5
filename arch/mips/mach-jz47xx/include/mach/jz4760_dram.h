/*
 * JZ4760 DDR initialization - parameters definitions
 *
 * Copyright (c) 2015 Imagination Technologies
 * Author: Matt Redfearn <matt.redfearn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __JZ4760_DRAM_H__
#define __JZ4760_DRAM_H__
#include "jz4760.h"

/*
 * DDR
 */
#define DDRC_ST				(DDRC_BASE + 0x00)
#define DDRC_CFG			(DDRC_BASE + 0x04)
#define DDRC_CTRL			(DDRC_BASE + 0x08)
#define DDRC_LMR			(DDRC_BASE + 0x0c)
#define DDRC_TIMING1		(DDRC_BASE + 0x10)
#define DDRC_TIMING2		(DDRC_BASE + 0x14)
#define DDRC_REFCNT			(DDRC_BASE + 0x18)
#define DDRC_DQS			(DDRC_BASE + 0x1c)
#define DDRC_DQSADJ			(DDRC_BASE + 0x20)
#define DDRC_MMAP0			(DDRC_BASE + 0x24)
#define DDRC_MMAP1			(DDRC_BASE + 0x28)
#define DDRC_MDELAY			(DDRC_BASE + 0x2c)

/* DDRC Status Register */
#define DDRC_ST_ENDIAN			BIT(7)
#define DDRC_ST_DPDN			BIT(5)
#define DDRC_ST_PDN				BIT(4)
#define DDRC_ST_AREF			BIT(3)
#define DDRC_ST_SREF			BIT(2)
#define DDRC_ST_CKE1			BIT(1)
#define DDRC_ST_CKE0			BIT(0)

/* DDRC Configure Register */
#define DDRC_CFG_SDR			BIT(31)
#define DDRC_CFG_RDPRI			BIT(29)
#define DDRC_CFG_ROW1_BIT		27
#define DDRC_CFG_ROW1_MASK		(0x3 << DDRC_CFG_ROW1_BIT)
#define DDRC_CFG_COL1_BIT		24
#define DDRC_CFG_COL1_MASK		(0x3 << DDRC_CFG_COL1_BIT)
#define DDRC_CFG_BA1			BIT(24)
#define DDRC_CFG_IMBA			BIT(23)
#define DDRC_CFG_DQSMD			BIT(22)
#define DDRC_CFG_BTRUN			BIT(21)

#define DDRC_CFG_MISPE			BIT(15)

/* 
 * The MEM_TYPE register is unimplemented on JZ4760, so this does
 * nothing.
 */
#define DDRC_CFG_TYPE_BIT		12
#define DDRC_CFG_TYPE_MASK		(0x7 << DDRC_CFG_TYPE_BIT)
#define DDRC_CFG_TYPE_SDR		(0 << DDRC_CFG_TYPE_BIT)
#define DDRC_CFG_TYPE_MSDR		(1 << DDRD_CFG_TYPE_BIT)
#define DDRC_CFG_TYPE_DDR1		(2 << DDRC_CFG_TYPE_BIT)
#define DDRC_CFG_TYPE_MDDR		(3 << DDRC_CFG_TYPE_BIT)
#define DDRC_CFG_TYPE_DDR2		(4 << DDRC_CFG_TYPE_BIT)
#define DDRC_CFG_TYPE_DDR3		(6 << DDRC_CFG_TYPE_BIT)
#define DDRC_CFG_TYPE_MDDR3		(7 << DDRC_CFG_TYPE_BIT)


#define DDRC_CFG_ROW_BIT		11
#define DDRC_CFG_ROW_MASK		(0x3 << DDRC_CFG_ROW_BIT)
#define DDRC_CFG_ROW_12			(0 << DDRC_CFG_ROW_BIT)
#define DDRC_CFG_ROW_13			(1 << DDRC_CFG_ROW_BIT)
#define DDRC_CFG_ROW_14			(2 << DDRC_CFG_ROW_BIT)

#define DDRC_CFG_COL_BIT		8
#define DDRC_CFG_COL_MASK		(0x3 << DDRC_CFG_COL_BIT)
#define DDRC_CFG_COL_8			(0 << DDRC_CFG_COL_BIT)
#define DDRC_CFG_COL_9			(1 << DDRC_CFG_COL_BIT)
#define DDRC_CFG_COL_10			(2 << DDRC_CFG_COL_BIT)
#define DDRC_CFG_COL_11			(3 << DDRC_CFG_COL_BIT)

#define DDRC_CFG_CS1EN			BIT(7)
#define DDRC_CFG_CS0EN			BIT(6)
#define DDRC_CFG_CL_BIT			2
#define DDRC_CFG_CL_MASK		(0xf << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_1			(0 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_1_5			(1 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_2			(9 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_2_5			(2 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_3			(10 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_3_5			(3 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_4			(11 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_4_5			(4 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_5			(12 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_5_5			(5 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_6			(13 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_6_5			(6 << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_7			(14 << DDRC_CFG_CL_BIT)

#define DDRC_CFG_BA			BIT(1)
#define DDRC_CFG_DW			BIT(0)


/* DDRC Control Register */
#define DDRC_CTRL_PDT_BIT		12
#define DDRC_CTRL_PDT_MASK		(0x7 << DDRC_CTRL_PDT_BIT)
#define DDRC_CTRL_PDT_DIS		(0 << DDRC_CTRL_PDT_BIT)
#define DDRC_CTRL_PDT_8			(1 << DDRC_CTRL_PDT_BIT)
#define DDRC_CTRL_PDT_16		(2 << DDRC_CTRL_PDT_BIT)
#define DDRC_CTRL_PDT_32		(3 << DDRC_CTRL_PDT_BIT)
#define DDRC_CTRL_PDT_64		(4 << DDRC_CTRL_PDT_BIT)
#define DDRC_CTRL_PDT_128		(5 << DDRC_CTRL_PDT_BIT)

#define DDRC_CTRL_ACTSTP		BIT(11)
#define DDRC_CTRL_SPD			BIT(7)
#define DDRC_CTRL_DPD			BIT(6)
#define DDRC_CTRL_SR			BIT(5)
#define DDRC_CTRL_UNALIGN		BIT(4)
#define DDRC_CTRL_ALH			BIT(3)
#define DDRC_CTRL_CKE			BIT(1)
#define DDRC_CTRL_RESET			BIT(0)

/* DDRC Load-Mode-Register */
#define DDRC_LMR_DDR_ADDR_BIT		16
#define DDRC_LMR_DDR_ADDR_MASK		(0x3fff << DDRC_LMR_DDR_ADDR_BIT)

#define DDRC_LMR_BA_BIT			8
#define DDRC_LMR_BA_MASK		(0x7 << DDRC_LMR_BA_BIT)
/* For DDR2 */
#define DDRC_LMR_BA_MRS			(0 << DDRC_LMR_BA_BIT)
#define DDRC_LMR_BA_EMRS1		(1 << DDRC_LMR_BA_BIT)
#define DDRC_LMR_BA_EMRS2		(2 << DDRC_LMR_BA_BIT)
#define DDRC_LMR_BA_EMRS3		(3 << DDRC_LMR_BA_BIT)
/* For mobile DDR */
#define DDRC_LMR_BA_M_MRS		(0 << DDRC_LMR_BA_BIT)
#define DDRC_LMR_BA_M_EMRS		(2 << DDRC_LMR_BA_BIT)
#define DDRC_LMR_BA_M_SR		(1 << DDRC_LMR_BA_BIT)
/* For Normal DDR1 */
#define DDRC_LMR_BA_N_MRS		(0 << DDRC_LMR_BA_BIT)
#define DDRC_LMR_BA_N_EMRS		(1 << DDRC_LMR_BA_BIT)

#define DDRC_LMR_CMD_BIT		4
#define DDRC_LMR_CMD_MASK		(0x3 << DDRC_LMR_CMD_BIT)
#define DDRC_LMR_CMD_PREC		(0 << DDRC_LMR_CMD_BIT)
#define DDRC_LMR_CMD_AUREF		(1 << DDRC_LMR_CMD_BIT)
#define DDRC_LMR_CMD_LMR		(2 << DDRC_LMR_CMD_BIT)

#define DDRC_LMR_START			BIT(0)

/* DDRC Timing Config Register 1 */
#define DDRC_TIMING1_TRAS_BIT	28
#define DDRC_TIMING1_TRAS_MASK	(0x7 << DDRC_TIMING1_TRAS_BIT)
#define DDRC_TIMING1_TRAS_DIS	(0 << DDRC_TIMING1_TRAS_BIT)
#define DDRC_TIMING1_TRAS(n)	((2 * n + 1) << DDRC_TIMING1_TRAS_BIT)

#define DDRC_TIMING1_TRTP_BIT	24
#define DDRC_TIMING1_TRTP_MASK	(0x3 << DDRC_TIMING1_TRTP_BIT)
#define DDRC_TIMING1_TRTP(n)	((n - 1) << DDRC_TIMING1_TRTP_BIT)

#define DDRC_TIMING1_TRP_BIT	20
#define DDRC_TIMING1_TRP_MASK	(0x7 << DDRC_TIMING1_TRP_MASK)
#define DDRC_TIMING1_TRP(n)		((n - 1) << DDRC_TIMING1_TRP_BIT)

#define DDRC_TIMING1_TRCD_BIT	16
#define DDRC_TIMING1_TRCD_MASK	(0x7 << DDRC_TIMING1_TRCD_MASK)
#define DDRC_TIMING1_TRCD(n)	((n - 1) << DDRC_TIMING1_TRCD_BIT)

#define DDRC_TIMING1_TRC_BIT	12
#define DDRC_TIMING1_TRC_MASK	(0xf << DDRC_TIMING1_TRC_BIT)
#define DDRC_TIMING1_TRC_DIS	(0 << DDRC_TIMING1_TRC_BIT)
#define DDRC_TIMING1_TRC(n)		((2 * n + 1) << DDRC_TIMING1_TRC_BIT)

#define DDRC_TIMING1_TRRD_BIT	8
#define DDRC_TIMING1_TRRD_MASK	(0x7 << DDRC_TIMING1_TRRD_BIT)
#define DDRC_TIMING1_TRRD_DIS	(0 << DDRC_TIMING1_TRRD_BIT)
#define DDRC_TIMING1_TRRD_2		(1 << DDRC_TIMING1_TRRD_BIT)
#define DDRC_TIMING1_TRRD_3		(2 << DDRC_TIMING1_TRRD_BIT)
#define DDRC_TIMING1_TRRD_4		(3 << DDRC_TIMING1_TRRD_BIT)

#define DDRC_TIMING1_TWR_BIT	4
#define DDRC_TIMING1_TWR_MASK	(0x7 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWR_1		(0 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWR_2		(1 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWR_3		(2 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWR_4		(3 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWR_5		(4 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWR_6		(5 << DDRC_TIMING1_TWR_BIT)

#define DDRC_TIMING1_TWTR_BIT	0
#define DDRC_TIMING1_TWTR_MASK	(0x3 << DDRC_TIMING1_TWTR_BIT)
#define DDRC_TIMING1_TWTR_1		(0 << DDRC_TIMING1_TWTR_BIT)
#define DDRC_TIMING1_TWTR_2		(1 << DDRC_TIMING1_TWTR_BIT)
#define DDRC_TIMING1_TWTR_3		(2 << DDRC_TIMING1_TWTR_BIT)
#define DDRC_TIMING1_TWTR_4		(3 << DDRC_TIMING1_TWTR_BIT)

/* DDRC Timing Config Register 2 */
#define DDRC_TIMING2_TCKE_BIT		16
#define DDRC_TIMING2_TCKE_MASK		(0x7 << DDRC_TIMING2_TCKE_BIT)
#define DDRC_TIMING2_TRFC_BIT		12
#define DDRC_TIMING2_TRFC_MASK		(0xf << DDRC_TIMING2_TRFC_BIT)
#define DDRC_TIMING2_TMINSR_BIT		8
#define DDRC_TIMING2_TMINSR_MASK	(0xf << DDRC_TIMING2_TMINSR_BIT)
#define DDRC_TIMING2_TXP_BIT		4
#define DDRC_TIMING2_TXP_MASK		(0x7 << DDRC_TIMING2_TXP_BIT)
#define DDRC_TIMING2_TMRD_BIT		0
#define DDRC_TIMING2_TMRD_MASK		(0x3 << DDRC_TIMING2_TMRD_BIT)

/* DDRC  Auto-Refresh Counter */
#define DDRC_REFCNT_CON_BIT			16
#define DDRC_REFCNT_CON_MASK		(0xff << DDRC_REFCNT_CON_BIT)
#define DDRC_REFCNT_CNT_BIT			8
#define DDRC_REFCNT_CNT_MASK		(0xff << DDRC_REFCNT_CNT_BIT)
#define DDRC_REFCNT_CLKDIV_BIT		1
#define DDRC_REFCNT_CLKDIV_MASK		(0x7 << DDRC_REFCNT_CLKDIV_BIT)
#define DDRC_REFCNT_REF_EN			BIT(0)

/* DDRC DQS Delay Control Register */
#define DDRC_DQS_ERROR			BIT(29)
#define DDRC_DQS_READY			BIT(28)
#define DDRC_DQS_SRDET			BIT(25)
#define DDRC_DQS_DET			BIT(24)
#define DDRC_DQS_AUTO			BIT(23)
#define DDRC_DQS_CLKD_BIT		16
#define DDRC_DQS_CLKD_MASK		(0x3f << DDRC_DQS_CLKD_BIT)
#define DDRC_DQS_WDQS_BIT		8
#define DDRC_DQS_WDQS_MASK		(0x3f << DDRC_DQS_WDQS_BIT)
#define DDRC_DQS_RDQS_BIT		1
#define DDRC_DQS_RDQS_MASK		(0x3f << DDRC_DQS_RDQS_BIT)

/* DDRC DQS Delay Adjust Register */
#define DDRC_DQSADJ_WDQS_BIT		8
#define DDRC_DQSADJ_WDQS_MASK		(0x1f << DDRC_DQS_ADJWDQS_BIT)
#define DDRC_DQSADJ_RDQS_BIT		0
#define DDRC_DQSADJ_RDQS_MASK		(0x1f << DDRC_DQS_ADJRDQS_BIT)

/* DDRC Memory Map Config Register */
#define DDRC_MMAP_BASE_BIT		8
#define DDRC_MMAP_BASE_MASK		(0xff << DDRC_MMAP_BASE_BIT)
#define DDRC_MMAP_MASK_BIT		0
#define DDRC_MMAP_MASK_MASK		(0xff << DDRC_MMAP_MASK_BIT)

/*********/

/* DDRP DRAM Configuration Register */
#define DDRP_DCR_TYPE_BIT		0
#define DDRP_DCR_TYPE_MASK		(0x7 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_TYPE_MDDR		(0 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_TYPE_DDR		(1 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_TYPE_DDR2		(2 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_TYPE_DDR3		(3 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_TYPE_LPDDR2		(4 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_DDR8BNK_BIT		3
#define DDRP_DCR_DDR8BNK_MASK		(1 << DDRP_DCR_DDR8BNK_BIT)
#define DDRP_DCR_DDR8BNK		(1 << DDRP_DCR_DDR8BNK_BIT)
#define DDRP_DCR_DDR8BNK_DIS		(0 << DDRP_DCR_DDR8BNK_BIT)

#define DRP_DTRP1_RTODT			BIT(11)

#define DDRP_DXGCR_DXEN			BIT(0)

#define DDRP_ZQXCR_ZDEN_BIT		28
#define DDRP_ZQXCR_ZDEN			(1 << DDRP_ZQXCR_ZDEN_BIT)
#define DDRP_ZQXCR_PULLUP_IMPE_BIT	5
#define DDRP_ZQXCR_PULLDOWN_IMPE_BIT	0

/* DDR3 Mode Register Set */
#define DDR3_MR0_BL_BIT			0
#define DDR3_MR0_BL_MASK		(3 << DDR3_MR0_BL_BIT)
#define DDR3_MR0_BL_8			(0 << DDR3_MR0_BL_BIT)
#define DDR3_MR0_BL_fly			(1 << DDR3_MR0_BL_BIT)
#define DDR3_MR0_BL_4			(2 << DDR3_MR0_BL_BIT)
#define DDR3_MR0_BT_BIT			3
#define DDR3_MR0_BT_MASK		(1 << DDR3_MR0_BT_BIT)
#define DDR3_MR0_BT_SEQ			(0 << DDR3_MR0_BT_BIT)
#define DDR3_MR0_BT_INTER		(1 << DDR3_MR0_BT_BIT)
#define DDR3_MR0_WR_BIT			9

#define DDR3_MR1_DLL_DISABLE		1
#define DDR3_MR1_DIC_6			(0 << 5 | 0 << 1)
#define DDR3_MR1_DIC_7			(0 << 5 | BIT(1))
#define DDR3_MR1_RTT_DIS		(0 << 9 | 0 << 6 | 0 << 2)
#define DDR3_MR1_RTT_4			(0 << 9 | 0 << 6 | BIT(2))
#define DDR3_MR1_RTT_2			(0 << 9 | BIT(6) | 0 << 2)
#define DDR3_MR1_RTT_6			(0 << 9 | BIT(6) | BIT(2))
#define DDR3_MR1_RTT_12			(BIT(9) | 0 << 6 | 0 << 2)
#define DDR3_MR1_RTT_8			(BIT(9) | 0 << 6 | BIT(2))

#define DDR3_MR2_CWL_BIT		3

/* Paramters common to all RAM devices used */

/* Chip Select */
/* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */
#define DDR_CS0EN	1
/* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */
#define DDR_CS1EN	0

/* ROW : 12 to 18 row address, 1G only 512MB */
#define DDR_ROW		15
/* COL :  8 to 14 column address */
#define DDR_COL		10
/* Banks each chip: 0-4bank, 1-8bank */
#define DDR_BANK8	1
/* 0 - 16-bit data width, 1 - 32-bit data width */
#define DDR_DW32	1

/* Refresh period: 64ms / 32768 = 1.95 us , 2 ^ 15 = 32768 */
#define DDR_tREFI	7800
/* Clock Divider */
#define DDR_CLK_DIV	1

/* DDR3 Burst length: 0 - 8 burst, 2 - 4 burst , 1 - 4 or 8 (on the fly) */
#define DDR_BL		8

/* CAS latency: 5 to 14, tCK */
#define DDR_CL		6
/* DDR3 only: CAS Write Latency, 5 to 8 */
#define DDR_tCWL	(DDR_CL - 1)

#endif	/* __JZ4760_DRAM_H__ */

