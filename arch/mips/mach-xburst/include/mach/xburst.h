/*
 *  linux/include/mach-mips/jzsoc.h
 *
 *  Ingenic's JZXXXX SoC common include.
 *
 *  Copyright (C) 2006 - 2008 Ingenic Semiconductor Inc.
 *
 *  Author: <jlwei@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_JZSOC_H__
#define __ASM_JZSOC_H__

/*
 * SoC include
 */

#ifdef CONFIG_JZ4730
#include <mach/jz4730.h>
#endif

#ifdef CONFIG_JZ4740
#include <mach/jz4740.h>
#endif

#ifdef CONFIG_JZ4750
#include <mach/jz4750.h>
#endif

#ifdef CONFIG_JZ4750D
#include <mach/jz4750d.h>
#endif

#ifdef CONFIG_JZ4760
#include <mach/jz4760.h>
#endif

#ifdef CONFIG_JZ4760B
#include <mach/jz4760b.h>
#endif

#ifdef CONFIG_JZ4770
#include <mach/jz4770.h>
#endif

#ifdef CONFIG_JZ4810
#include <mach/jz4810.h>
#endif

#endif /* __ASM_JZSOC_H__ */
