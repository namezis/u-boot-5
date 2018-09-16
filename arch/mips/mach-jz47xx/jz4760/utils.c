#include <common.h>
#include <config.h>
#include <mach/jz4760.h>
#include <linux/delay.h>

void __udelay(unsigned long us)
{
    unsigned long tmp = (CONFIG_SYS_MHZ / 2) * us;
    asm volatile (
        ".set noreorder\n\t"
        "0:\n\t"
            "bnez %0,0b\n\t"
            "addiu %0, %0, -1\n\t"
        ".set reorder\n\t"
            ::"r"(tmp)
        );
}

