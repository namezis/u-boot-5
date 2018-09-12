// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/io.h>
//#include <mach/xburst.h>

void _machine_restart(void)
{
	// No idea how to reset this chip.
	while (1)
		;
}
