#include <common.h>
#include <asm/io.h>
#include <mach/jz4760.h>

uint32_t sdram_size(int cs) {
	const struct jz4760_ddr_config *cfg;
	u8 data_width, banks;
	u32 size;

	cfg = jz4760_get_ddr_config();

	data_width = (cfg->dw32 + 1) * 16;
	banks = (cfg->bank8 + 1) * 4;
	size = (1 << (cfg->row + cfg->col)) * data_width * banks;
	size *= (cfg->cs0 + cfg->cs1);

	if(size > 0x20000000) {
		size = 0x20000000;
	}
	return size;
}

void sdram_init(void) {
	const struct jz4760_ddr_config *cfg;
	u32 tREFI;

	cfg = jz4760_get_ddr_config();

	REG_DDRC_CFG = (
		);

	REG_DDRC_TIMING1 = (
		(cfg->timing.tRAS << DDRC_TIMING1_TRAS_BIT) |
		(cfg->timing.tRTP << DDRC_TIMING1_TRTP_BIT) |
		(cfg->timing.tRP << DDRC_TIMING1_TRP_BIT) |
		(cfg->timing.tRCD << DDRC_TIMING1_TRCD_BIT) |
		(cfg->timing.tRC << DDRC_TIMING1_TRC_BIT) |
		(cfg->timing.tRRD << DDRC_TIMING1_TRRD_BIT) |
		(cfg->timing.tWR << DDRC_TIMING1_TWR_BIT) |
		(cfg->timing.tWTR << DDRC_TIMING1_TWR_BIT)
	);

	REG_DDRC_TIMING2 = (
		(cfg->timing.tRFC << DDRC_TIMING2_TRFC_BIT) |
		(cfg->timing.tMINSR << DDRC_TIMING2_TMINSR_BIT) |
		(cfg->timing.tXP << DDRC_TIMING2_TXP_BIT) |
		(cfg->timing.tMRD << DDRC_TIMING2_TMRD_BIT)
	);

	REG_DDRC_MMAP0 = (
			);

	REG_DDRC_MMAP1 = 0x0000ff00;

	tREFI = (cfg->timing.tREFI / (1000000 / __cpm_get_mclk()));

	REG_DDRC_REFCNT = (
		(tREFI << DDRC_REFCNT_CON_BIT) |
		DDRC_REFCNT_REF_EN
}
