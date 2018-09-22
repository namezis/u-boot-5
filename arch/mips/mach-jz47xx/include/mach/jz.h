#ifndef JZ_H
#define JZ_H

#ifdef CONFIG_SOC_JZ4760
#include <mach/jz4760.h>
#elif CONFIG_SOC_JZ4780
#include <mach/jz4780.h>
#else
#error "No JZ SOC configured!"
#endif

#endif
