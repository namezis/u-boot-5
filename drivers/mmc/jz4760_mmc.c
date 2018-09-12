#include <common.h>
#include <dm.h>
#include <linux/libfdt.h>
#include <mmc.h>

struct jz4760_mmc_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};

struct jz4760_mmc_priv {
	fdt_addr_t base;
};

static int jz4760_mmc_send_cmd(struct udevice *dev, struct mmc_cmd *cmd, struct mmc_data *data) {
	return 0;
}

static int jz4760_mmc_set_ios(struct udevice *dev) {
	return 0;
}

static int jz4760_mmc_getcd(struct udevice *dev) {
	return 0;
}

static int jz4760_mmc_probe(struct udevice *dev) {
	return 0;
}

static int jz4760_mmc_bind(struct udevice *dev) {
	return 0;
}

static const struct dm_mmc_ops jz4760_mmc_ops = {
	.send_cmd = jz4760_mmc_send_cmd,
	.set_ios = jz4760_mmc_set_ios,
	.get_cd = jz4760_mmc_getcd,
};

static const struct udevice_id jz4760_mmc_ids[] = {
	{ .compatible = "ingenic,jz4760-mmc" },
	{ }
};

U_BOOT_DRIVER(jz4760_mmc) = {
	.name = "jz4760_mmc",
	.id = UCLASS_MMC,
	.of_match = jz4760_mmc_ids,
	.ops = &jz4760_mmc_ops,
	.probe = &jz4760_mmc_probe,
	.bind = &jz4760_mmc_bind,
	.priv_auto_alloc_size = sizeof(struct jz4760_mmc_priv),
	.platdata_auto_alloc_size = sizeof(struct jz4760_mmc_plat),
};
