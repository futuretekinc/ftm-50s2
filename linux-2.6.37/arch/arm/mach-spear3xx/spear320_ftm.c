/*
 * arch/arm/mach-spear3xx/spear320_ftm.c
 *
 * SPEAr320 evaluation board source file
 *
 * Copyright (C) 2009 ST Microelectronics
 * Viresh Kumar<viresh.kumar@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/gpio.h>
#include <linux/mmc/sdhci-spear.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/fsmc.h>
#include <linux/phy.h>
#include <linux/spi/flash.h>
#include <linux/spi/spi.h>
#include <linux/stmmac.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <plat/fsmc.h>
#include <plat/smi.h>
#include <plat/spi.h>
#include <mach/emi.h>
#include <mach/generic.h>
#include <mach/hardware.h>
#include <mach/macb_eth.h>
#include <mach/misc_regs.h>

#define PARTITION(n, off, sz)	{.name = n, .offset = off, .size = sz}

static struct mtd_partition partition_info[] = {
	PARTITION("X-loader",	0x000000,  0x10000),
	PARTITION("U-Boot",		0x010000,  0x40000),
	PARTITION("Kernel",		0x050000,  0x2c000),
	PARTITION("ramdisk",	0x310000,  0x4F000)
};

/* emi nor flash resources registeration */
static struct resource emi_nor_resources[] = {
	{
		.start	= SPEAR320_EMI_MEM_0_BASE,
		.end	= SPEAR320_EMI_MEM_0_BASE + SPEAR320_EMI_MEM_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

/* ethernet phy device */
static struct plat_stmmacphy_data phy_private_data = {
	.bus_id = 0,
#if CONFIG_MACH_FTM_50S2
	.phy_addr = 0,
#else	
	.phy_addr = 7,
#endif
	.phy_mask = 0,
	.interface = PHY_INTERFACE_MODE_MII,
};

static struct resource phy_resources = {
	.name = "phyirq",
	.start = -1,
	.end = -1,
	.flags = IORESOURCE_IRQ,
};

static struct platform_device spear320_phy_device = {
	.name = "stmmacphy",
	.id = -1,
	.num_resources = 1,
	.resource = &phy_resources,
	.dev.platform_data = &phy_private_data,
};

/* Ethernet Private data */
static struct macb_base_data spear320_macb_data = {
	.bus_id = 1,
	.phy_mask = 0,
	.gpio_num = PLGPIO_54,
	.phy_addr = 0,
	.mac_addr = {0xf2, 0xf2, 0xf2, 0x45, 0x67, 0x89},
	.plat_mdio_control = spear3xx_macb_plat_mdio_control,
};

/* padmux devices to enable */
static struct pmx_dev *pmx_devs[] = {
	/* spear3xx specific devices */
	&spear3xx_pmx_i2c,
//	&spear3xx_pmx_ssp,
	&spear3xx_pmx_mii,
	&spear3xx_pmx_uart0,

	/* spear320 specific devices */
	&spear320s_pmx_fsmc[0],
	&spear320_pmx_uart1,
	&spear320_pmx_uart2,
	&spear320s_pmx_mii2,
	&spear320_pmx_sdhci[1],
	&spear320_pmx_pwm0_1[0],
	&spear320_pmx_pwm2[0],
};

static struct amba_device *amba_devs[] __initdata = {
	/* spear3xx specific devices */
	&spear3xx_dma_device,
	&spear3xx_gpio_device,
//	&spear3xx_ssp0_device,
	&spear3xx_uart_device,
	&spear3xx_wdt_device,

	/* spear320 specific devices */
	&spear320_uart1_device,
	&spear320_uart2_device,
};

static struct platform_device *plat_devs[] __initdata = {
	/* spear3xx specific devices */
	&spear3xx_adc_device,
	&spear3xx_cpufreq_device,
	&spear3xx_ehci_device,
	&spear3xx_eth_device,
	&spear3xx_i2c_device,
	&spear3xx_jpeg_device,
	&spear3xx_ohci0_device,
	&spear3xx_ohci1_device,
	&spear3xx_rtc_device,
	&spear3xx_smi_device,
	&spear3xx_udc_device,

	/* spear320 specific devices */
	&spear320_eth1_device,
	&spear320_i2c1_device,
	&spear320_nand_device,
	&spear320_phy_device,
	&spear320_plgpio_device,
	&spear320_pwm_device,
	&spear320_sdhci_device
};

/* Currently no gpios are free on eval board so it is kept commented */
#if 0
/* spi0 flash Chip Select Control function, controlled by gpio pin mentioned */
DECLARE_SPI_CS_GPIO_CONTROL(0, flash, /* mention gpio number here */);
/* spi0 flash Chip Info structure */
DECLARE_SPI_CHIP_INFO(0, flash, spi0_flash_cs_gpio_control);

/* spi0 spidev Chip Select Control function, controlled by gpio pin mentioned */
DECLARE_SPI_CS_GPIO_CONTROL(0, dev, /* mention gpio number here */);
/* spi0 spidev Chip Info structure */
DECLARE_SPI_CHIP_INFO(0, dev, spi0_dev_cs_gpio_control);
#endif

static struct spi_board_info __initdata spi_board_info[] = {
#if 0
	/* spi0 board info */
	{
		.modalias = "spidev",
		.controller_data = &spi0_dev_chip_info,
		.max_speed_hz = 25000000,
		.bus_num = 0,
		.chip_select = 0,
		.mode = SPI_MODE_1,
	}, {
		.modalias = "m25p80",
		.controller_data = &spi0_flash_chip_info,
		.max_speed_hz = 22000000, /* Actual 20.75 */
		.bus_num = 0,
		.chip_select = 1,
		.mode = SPI_MODE_3,
	}
#endif
};

/* sdhci board specific information */
static struct sdhci_plat_data sdhci_plat_data = {
	.card_power_gpio = PLGPIO_61,
	.power_active_high = 0,
	.power_always_enb = 1,
	.card_int_gpio = -1,
};

static void __init spear320_ftm_init(void)
{
	unsigned int i;

	/* set sdhci device platform data */
	sdhci_set_plat_data(&spear320_sdhci_device, &sdhci_plat_data);

	/* set nand device's plat data */
	fsmc_nand_set_plat_data(&spear320_nand_device, NULL, 0,
			NAND_SKIP_BBTSCAN, FSMC_NAND_BW8, NULL, 1);

	/* initialize macb related data in macb plat data */
	spear3xx_macb_setup();
	macb_set_plat_data(&spear320_eth1_device, &spear320_macb_data);

	/* call spear320 machine init function */
	spear320_common_init(&spear320_auto_net_mii_mode, pmx_devs,
			ARRAY_SIZE(pmx_devs));

	/* initialize serial nor related data in smi plat data */
	smi_init_board_info(&spear3xx_smi_device);

	/* Register slave devices on the I2C buses */
	i2c_register_default_devices();

	/* initialize emi related data in emi plat data */
	emi_init_board_info(&spear320_emi_nor_device, emi_nor_resources,
			ARRAY_SIZE(emi_nor_resources), partition_info,
			ARRAY_SIZE(partition_info), EMI_FLASH_WIDTH16);

	/* Initialize emi regiters */
	emi_init(&spear320_emi_nor_device, SPEAR320_EMI_CTRL_BASE, 0,
			EMI_FLASH_WIDTH16);

	/* Add Platform Devices */
	platform_add_devices(plat_devs, ARRAY_SIZE(plat_devs));

	/* Add Amba Devices */
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++)
		amba_device_register(amba_devs[i], &iomem_resource);

	spi_register_board_info(spi_board_info, ARRAY_SIZE(spi_board_info));
}

void get_ethaddr(int id, u8 *addr)
{
	extern char *saved_command_line;
	static int	first = 1;
	static unsigned int ethaddr[2][6];


	if (first)
	{
		char *str = strstr(saved_command_line, "ethaddr=");
		if (str != NULL)
		{
			sscanf(str+8, "%x:%x:%x:%x:%x:%x", 
				&ethaddr[0][0], &ethaddr[0][1],
				&ethaddr[0][2], &ethaddr[0][3],
				&ethaddr[0][4], &ethaddr[0][5]);
		}

		str = strstr(saved_command_line, "eth1addr=");
		if (str != NULL)
		{
			sscanf(str+9, "%x:%x:%x:%x:%x:%x", 
				&ethaddr[1][0], &ethaddr[1][1],
				&ethaddr[1][2], &ethaddr[1][3],
				&ethaddr[1][4], &ethaddr[1][5]);

		}

		first = 0;
	}

	if (0<= id && id < 2)
	{
		int	i;
		for(i = 0 ; i < 6 ; i++)
		{
			addr[i] = (char)ethaddr[id][i];
		}
	}
}

MACHINE_START(SPEAR320_FTM, "FutureTek-M2M")
	.boot_params	=	0x00000100,
	.map_io		=	spear320_map_io,
	.init_irq	=	spear3xx_init_irq,
	.timer		=	&spear3xx_timer,
	.init_machine	=	spear320_ftm_init,
MACHINE_END
