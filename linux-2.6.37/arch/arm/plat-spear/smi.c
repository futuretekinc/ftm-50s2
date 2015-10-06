/*
 * arch/arm/plat-spear/smi.c
 *
 * spear smi platform intialization
 *
 * Copyright (C) 2010 ST Microelectronics
 * Shiraz Hashim <shiraz.hashim@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <asm/mach-types.h>
#include <plat/smi.h>
#include <mach/hardware.h>

/*
 * physical base address of flash/bank mem map base associated with smi
 * depends on SoC
 */

#if defined(CONFIG_ARCH_SPEAR13XX)
#define FLASH_MEM_BASE	SPEAR13XX_SMI_MEM0_BASE

#elif defined(CONFIG_ARCH_SPEAR3XX)
#define FLASH_MEM_BASE	SPEAR3XX_ICM3_SMEM_BASE

#elif defined(CONFIG_ARCH_SPEAR6XX)
#define FLASH_MEM_BASE	SPEAR6XX_ICM3_SMEM_BASE

#endif

#if defined(CONFIG_MACH_FTM_50S2)
/* serial nor flash specific board data */
static struct mtd_partition m25p64_partition_info1[] = {
	DEFINE_PARTS("XLoader",	0x0000000,  0x00010000),
	DEFINE_PARTS("UBoot",	0x0010000,  0x00040000),
	DEFINE_PARTS("Kernel",	0x0050000,  0x002C0000),
	DEFINE_PARTS("InitRD",	0x0310000,  0x004F0000),
};
static struct mtd_partition m25p64_partition_info2[] = {
	DEFINE_PARTS("User",	0x0000000,  0x00800000)
};
static struct spear_smi_flash_info nor_flash_info[] = {
	{
		.name = "m25p64",
		.fast_mode = 1,
		.mem_base = FLASH_MEM_BASE,
		.size = 8 * 1024 * 1024,
		.num_parts = ARRAY_SIZE(m25p64_partition_info1),
		.parts = m25p64_partition_info1,
	},
	{
		.name = "m25p64",
		.fast_mode = 1,
		.mem_base = FLASH_MEM_BASE + 0x1000000,
		.size = 8 * 1024 * 1024,
		.num_parts = ARRAY_SIZE(m25p64_partition_info2),
		.parts = m25p64_partition_info2,
	},
};
#else
static struct mtd_partition m25p64_partition_info[] = {
	DEFINE_PARTS("XLoader",	0x0000000,  0x00010000),
	DEFINE_PARTS("UBoot",	0x0010000,  0x00040000),
	DEFINE_PARTS("Kernel",	0x0050000,  0x00380000),
	DEFINE_PARTS("Kernel",	0x03D0000,  0x00380000),
	DEFINE_PARTS("SysInfo",	0x07D0000,  0x00030000)
};

static struct spear_smi_flash_info nor_flash_info[] = {
	{
		.name = "m25p64",
		.fast_mode = 1,
		.mem_base = FLASH_MEM_BASE,
		.size = 8 * 1024 * 1024,
		.num_parts = ARRAY_SIZE(m25p64_partition_info),
		.parts = m25p64_partition_info,
	},
};
#endif

/* smi specific board data */
static struct spear_smi_plat_data smi_plat_data = {
	.clk_rate = 50000000,	/* 50MHz */
	.num_flashes = ARRAY_SIZE(nor_flash_info),
	.board_flash_info = nor_flash_info,
};

void smi_init_board_info(struct platform_device *pdev)
{
	smi_set_plat_data(pdev, &smi_plat_data);
}
