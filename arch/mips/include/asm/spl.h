/*
 * (C) Copyright 2012
 * Texas Instruments, <www.ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef	_ASM_SPL_H_
#define	_ASM_SPL_H_

enum {
	BOOT_DEVICE_RAM,
	BOOT_DEVICE_MMC1,
	BOOT_DEVICE_MMC2,
	BOOT_DEVICE_MMC2_2,
	BOOT_DEVICE_NAND,
	BOOT_DEVICE_ONENAND,
	BOOT_DEVICE_NOR,
	BOOT_DEVICE_UART,
	BOOT_DEVICE_SPI,
	BOOT_DEVICE_USB,
	BOOT_DEVICE_SATA,
	BOOT_DEVICE_I2C,
	BOOT_DEVICE_BOARD,
	BOOT_DEVICE_NONE
};

/* Linker symbols. */
extern char __bss_start[];
extern ulong __bss_end;

#ifndef CONFIG_DM
extern gd_t gdata;
#endif

#endif
