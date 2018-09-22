/*
 * JZ4760 debug UART
 *
 * Copyright (c) 2018 Legitimate Data Company
 * Author: Jeremy Grosser <jeremy@synack.me>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <common.h>
#include <config.h>
#include <mach/jz4760.h>
#include <serial.h>
#include <ns16550.h>
#include <debug_uart.h>

#define DUART_BASE		UART1_BASE
#define DUART_DLLR		REG8(DUART_BASE + OFF_DLLR)
#define DUART_DLHR		REG8(DUART_BASE + OFF_DLHR)
#define DUART_LCR		REG8(DUART_BASE + OFF_LCR)
#define DUART_FCR		REG8(DUART_BASE + OFF_FCR)
#define DUART_TDR		REG8(DUART_BASE + OFF_TDR)
#define DUART_LSR		REG8(DUART_BASE + OFF_LSR)

#ifdef CONFIG_DEBUG_UART_JZ4760

#define BAUD_DIV	((CONFIG_SYS_NS16550_CLK >> 4) / CONFIG_BAUDRATE)
static inline void _debug_uart_init(void) {
	// UART is driven by EXCLK / 1
	REG_CPM_CPPCR &= ~CPM_CPCCR_ECS;

	__gpio_as_uart1();
	__cpm_start_uart1();

	DUART_LCR |= UART_LCR_DLAB;

	DUART_DLHR = (BAUD_DIV >> 8) & 0xFF;
	DUART_DLLR = BAUD_DIV & 0xFF;
	DUART_LCR = UART_LCR_WLS_8; // 8n1

	// Enable UART with FIFO
	DUART_FCR |= (UART_FCR_FIFO_EN | UART_FCR_UME);
}

static inline void _debug_uart_putc(int ch) {
	char c = (char)ch;
	if(c == '\n') {
		printch('\r');
	}
	DUART_TDR = c;
	while((DUART_LSR & UART_LSR_THRE) == 0);
}

DEBUG_UART_FUNCS

#endif

void debug_print(const char *msg) {
	printascii(msg);
	/*
	int i;
	for(i = 0; i < strlen(msg); i++) {
		debug_putc(msg[i]);
	}
	*/
}

void debug_printhex(uint8_t c) {
	printhex2(c);
	/*
    int i;
    char v;
    for(i = 0; i < 2; i++) {
        v = (char)((c >> (4 * (1 - i))) & 0xF);
        if(v < 10) {
            debug_putc(v + '0');
        }else{
            debug_putc((v - 10) + 'A');
        }
    }
	*/
}

void debug_printhex4(uint32_t c) {
	printhex4(c);
	/*
    debug_print("0x");
	debug_printhex((c >> 24) & 0xff);
	debug_printhex((c >> 16) & 0xff);
	debug_printhex((c >> 8) & 0xff);
	debug_printhex(c & 0xff);
    debug_putc('\n');
	*/
}

/*
struct serial_device duart = {
	.setbrg = &debug_init,
	.putc = &debug_putc,
	.puts = &debug_print,
};

struct serial_device *default_serial_console(void) {
	return &eserial2_device;
}
*/
