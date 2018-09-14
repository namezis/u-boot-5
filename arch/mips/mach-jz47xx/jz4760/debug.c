#include <common.h>
#include <config.h>
#include <mach/jz4760.h>
#include <serial.h>

#define DUART_BASE		UART1_BASE
#define DUART_DLLR		REG8(DUART_BASE + OFF_DLLR)
#define DUART_DLHR		REG8(DUART_BASE + OFF_DLHR)
#define DUART_LCR		REG8(DUART_BASE + OFF_LCR)
#define DUART_FCR		REG8(DUART_BASE + OFF_FCR)
#define DUART_TDR		REG8(DUART_BASE + OFF_TDR)

void debug_init(void) {
	uint32_t baud_div;

	// UART is driven by EXCLK / 1
	REG_CPM_CPPCR &= ~CPM_CPCCR_ECS;

	__gpio_as_uart1();
	__cpm_start_uart1();

	DUART_LCR |= UART_LCR_DLAB;

	baud_div = (CONFIG_SYS_NS16550_CLK / 16 / CONFIG_BAUDRATE);
	DUART_DLHR = (baud_div >> 8) & 0xFF;
	DUART_DLLR = baud_div & 0xFF;
	DUART_LCR = UART_LCR_STOP_1 | UART_LCR_WLEN_8; // 8n1

	// Enable UART with FIFO
	DUART_FCR |= (UART_FCR_FE | UART_FCR_UUE);
}

void debug_putc(char ch) {
	DUART_TDR = ch;
}

void debug_print(const char *msg) {
	int i;
	for(i = 0; i < strlen(msg); i++) {
		if(msg[i] == '\n') {
			debug_putc('\r');
		}
		debug_putc(msg[i]);
	}
}


struct serial_device duart = {
	.setbrg = &debug_init,
	.putc = &debug_putc,
	.puts = &debug_print,
};

struct serial_device *default_serial_console(void) {
	return &duart;
}
