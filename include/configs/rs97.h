/*
 * RS-97 board configuration
 *
 * Copyright (c) 2018 Legitimate Data Company
 * Author: Jeremy Grosser <jeremy@synack.me>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#ifndef __CONFIG_RS97_H__
#define __CONFIG_RS97_H__

#define CONFIG_SKIP_LOWLEVEL_INIT

/* Ingenic JZ4760 clock configuration. */
#define CONFIG_SYS_HZ			1000
#define CONFIG_SYS_MHZ			528
#define CONFIG_SYS_MIPS_TIMER_FREQ	(CONFIG_SYS_MHZ * 1000000)

/* Memory configuration */
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(896 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)

#define CONFIG_SYS_SDRAM_BASE		0x80000000 /* cached (KSEG0) address */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000
#define CONFIG_SYS_LOAD_ADDR		0x80600000
#define CONFIG_LOADADDR				CONFIG_SYS_LOAD_ADDR
#define CONFIG_SYS_MEMTEST_START	0x80100000
#define CONFIG_SYS_MEMTEST_END		0x80800000

#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/* NS16550-ish UARTs */
#define CONFIG_SYS_NS16550_CLK		12000000
/*
#define CONFIG_SYS_NS16550_COM2		UART1_BASE
#define CONFIG_CONS_INDEX 2

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_NS16550_REG_SIZE 1
#endif
*/

/* Environment */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		((14 + 512) << 10)
#define CONFIG_ENV_OVERWRITE

/* Command line configuration. */
#define CONFIG_SYS_CBSIZE	1024		/* Console I/O buffer size */
#define CONFIG_SYS_MAXARGS	32		/* Max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
						/* Boot argument buffer size */
#define CONFIG_VERSION_VARIABLE			/* U-BOOT version */

/* Miscellaneous configuration options */
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)
#define CONFIG_BOOTARGS			"console=ttyS1,57600 rw rootwait"
#define CONFIG_BOOTCOMMAND		"echo fixme"

/* SPL */
#define CONFIG_SPL_STACK			0x80004000

#define CONFIG_SPL_TEXT_BASE		0x80000000
#define CONFIG_SPL_MAX_SIZE			0x4000

#define CONFIG_SPL_BSS_START_ADDR	CONFIG_SPL_STACK
#define CONFIG_SPL_BSS_MAX_SIZE		0x00002000 /* 512KB, arbitrary */

#define CONFIG_SPL_START_S_PATH		"arch/mips/mach-jz47xx"

#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR        0x1c    /* 14 KiB offset */

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_ABORT_ON_RAW_IMAGE
#define CONFIG_MMC_TINY
#undef CONFIG_DM_MMC
#undef CONFIG_BLK
#undef CONFIG_DM_MMC_OPS
#endif

#endif /* __CONFIG_RS97_H__ */
