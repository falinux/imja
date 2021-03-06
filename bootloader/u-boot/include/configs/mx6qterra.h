/*
 * opyright (C) 2010-2011 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the FALinux EM-IMX6DQ
 * and Freescale i.MX6Q Terra boards.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "mx6_common.h"
#define CONFIG_MX6
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_MACH_TYPE    3769

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SYS_GENERIC_BOARD

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN       	(10 * 1024 * 1024)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_MXC_GPIO
#define CONFIG_CMD_GPIO
#define CONFIG_CI_UDC
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_CDC
#define CONFIG_NETCONSOLE

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE            UART1_BASE
#define CONFIG_BOOTCMD_RETRY_COUNT      20

#define CONFIG_CMD_SF
#ifdef CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_SST
#define CONFIG_MXC_SPI
#define CONFIG_SF_DEFAULT_BUS  			0
#define CONFIG_SF_DEFAULT_CS   			0
#define CONFIG_SF_DEFAULT_SPEED 		25000000
#define CONFIG_SF_DEFAULT_MODE 			(SPI_MODE_0)
#endif

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED        	100000
#define CONFIG_I2C_EDID

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR  0x08

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR      	0

#define CONFIG_SYS_FSL_USDHC_NUM       	2

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION

#ifdef CONFIG_MX6Q
#define CONFIG_CMD_SATA
#endif

/*
 * SATA Configs
 */
#ifdef CONFIG_CMD_SATA
#define CONFIG_DWC_AHSATA
#define CONFIG_SYS_SATA_MAX_DEVICE  	1
#define CONFIG_DWC_AHSATA_PORT_ID   	0
#define CONFIG_DWC_AHSATA_BASE_ADDR 	SATA_ARB_BASE_ADDR
#define CONFIG_LBA48
#define CONFIG_LIBATA
#endif

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE            		ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE     		RGMII
#define CONFIG_ETHPRIME         		"FEC"
#define CONFIG_PHYLIB
#define CONFIG_FEC_MXC_PHYADDR  		1
#define CONFIG_PHY_ATHEROS
#define CONFIG_PHY_ATHEROS_AR8031

/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_CMD_FAT
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_ETHER_MCS7830
#define CONFIG_USB_ETHER_SMSC95XX
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET    /* For OTG port */
#define CONFIG_MXC_USB_PORTSC   		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS    		0
#define CONFIG_USB_KEYBOARD
#define CONFIG_SYS_USB_EVENT_POLL_VIA_CONTROL_EP

/* Miscellaneous commands */
#define CONFIG_CMD_BMODE
#define CONFIG_CMD_SETEXPR


/* Framebuffer and LCD */
#define CONFIG_VIDEO
#define CONFIG_VIDEO_IPUV3
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IPUV3_CLK 				260000000
#define CONFIG_CMD_HDMIDETECT
#define CONFIG_CONSOLE_MUX
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP


/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX          		1
#define CONFIG_BAUDRATE            		115200

/* Command definition */
#include <config_cmd_default.h>

#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY           		2

#define CONFIG_PREBOOT             		""

#define CONFIG_LOADADDR            		0x12000000
#define CONFIG_SYS_TEXT_BASE       		0x17800000

#ifdef CONFIG_CMD_SATA
#define CONFIG_DRIVE_SATA "sata "
#else
#define CONFIG_DRIVE_SATA
#endif

#ifdef CONFIG_CMD_MMC
#define CONFIG_DRIVE_MMC "mmc "
#else
#define CONFIG_DRIVE_MMC
#endif

#ifdef CONFIG_USB_STORAGE
#define CONFIG_DRIVE_USB "usb "
#else
#define CONFIG_DRIVE_USB
#endif

#define CONFIG_DRIVE_TYPES CONFIG_DRIVE_SATA CONFIG_DRIVE_MMC CONFIG_DRIVE_USB
#define CONFIG_UMSDEVS CONFIG_DRIVE_SATA CONFIG_DRIVE_MMC

#define CONFIG_EXTRA_ENV_SETTINGS \
    "console=ttymxc0\0" \
    "fdt_high=0xffffffff\0" \
    "initrd_high=0xffffffff\0" \
    "loadaddr=0x12000000\0" \
    "boot_fdt=try\0" \
    "bootdelay=2\0" \
    "video=mxcfb0:dev=ldb,LDB-ZJ070,if=RGB666,bpp=16 ldb=sin0 mxcfb1:dev=hdmi,1920x1080M@60,if=RGB24,bpp=32 fbmem=24M vmalloc=400M video=mxcfb2:off\0" \
    "bootargs=console=ttymxc0,115200 root=/dev/mmcblk3p1 rw --no-log rootfstype=ext4 rootdelay=5 video=${video} host=${serverip}\0" \
    "bootargs_mmc=setenv bootargs console=ttymxc0,115200 root=/dev/mmcblk3p1 rw --no-log rootfstype=ext4 rootdelay=5 ip0=${ipaddr} eth=${ethaddr} video=${video}\0" \
    "bootargs_ram=setenv bootargs console=ttymxc0,115200 root=/dev/ram0 rw --no-log initrd=0x1E000000,32M ramdisk=32768 video=${video} host=${serverip}\0" \
    "bootargs_sata=setenv bootargs console=ttymxc0,115200 root=/dev/sda1 rw --no-log rootfstype=ext4 rootwait ${video} host=${serverip}\0" \
    "bootcmd=run bootargs_ram bootram_dtb \0" \
    "bootmmc=mmc dev 0; mmcinfo; mmc read 0x12000000 1000 7000; bootm 0x12000000\0" \
    "bootmmc_dtb=mmc dev 0; mmcinfo; mmc read 0x11F00000 A00 100; mmc read 0x12000000 1000 4000; bootm 0x12000000 - 0x11F00000\0" \
    "bootram=mmc dev 0; mmcinfo; mmc read 0x12000000 1000 7000; mmc read 0x1E000000 7000 7000; bootm 0x12000000\0" \
    "bootram_dtb=mmc dev 0; mmcinfo; mmc read 0x11F00000 A00 100; mmc read 0x12000000 1000 4000; mmc read 0x1E000000 7000 7000; bootm 0x12000000 - 0x11F00000\0" \
    "dtb=tftpboot 0x11F00000 imx6q-terra-imja.dtb; mmc write 0x11F00000 A00 100\0" \
    "ethact=FEC\0" \
    "ethaddr=00:FA:14:06:03:07\0" \
    "ethprime=FEC\0" \
    "gatewayip=192.168.10.1\0" \
    "ip_dyn=yes\0" \
    "ipaddr=192.168.2.139\0" \
    "kernel=tftpboot 0x12000000 uImage.imx6.imja; mmc write 0x12000000 1000 4000\0" \
    "netmask=255.255.0.0\0" \
    "panel=ZJ070NA-01B\0" \
    "ramdisk=tftpboot 0x1E000000 ramdisk.imx6-imja-2.0-32M.gz; mmc write 0x1E000000 7000 7000\0" \
    "serverip=192.168.2.131\0" \
    "uboot=tftpboot 0x12000000 u-boot.imx.imja; mmc write 0x12000000 2 500\0" \



/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER

#define CONFIG_SYS_PROMPT               "TERRA > "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE               1024

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS         		48
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MEMTEST_START        0x10000000
#define CONFIG_SYS_MEMTEST_END          0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH      0x10800000

#define CONFIG_SYS_LOAD_ADDR            CONFIG_LOADADDR

#define CONFIG_SYS_HZ              		1000

#define CONFIG_CMDLINE_EDITING

/* Physical Memory Map */
#define PHYS_SDRAM          			MMDC0_ARB_BASE_ADDR
/*#define PHYS_SDRAM_SIZE     			(2u * 1024 * 1024 * 1024)*/

#define CONFIG_NR_DRAM_BANKS            1

#define CONFIG_SYS_SDRAM_BASE           PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR        IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE        IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
    (CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
    (CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE         	(8 * 1024)

#if defined(CONFIG_FALINUX)
#define CONFIG_ENV_IS_IN_MMC
#else
#define CONFIG_ENV_IS_IN_SPI_FLASH
#endif

#if defined(CONFIG_ENV_IS_IN_MMC)
#ifdef CONFIG_FALINUX
#define CONFIG_ENV_OFFSET           	( 36 * 32 * 1024)
#else
#define CONFIG_ENV_OFFSET           	(512 * 64 * 1024)
#endif
#define CONFIG_SYS_MMC_ENV_DEV      	0
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET           	(768 * 1024)
#define CONFIG_ENV_SECT_SIZE        	(8 * 1024)
#define CONFIG_ENV_SPI_BUS          	CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS           	CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE         	CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ       	CONFIG_SF_DEFAULT_SPEED
#endif

#define CONFIG_OF_LIBFDT
#define CONFIG_CMD_BOOTZ

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

#define CONFIG_CMD_BMP

#define CONFIG_CMD_TIME
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_ALT_MEMTEST

#define CONFIG_CMD_BOOTZ
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_CMD_FS_GENERIC

#define CONFIG_CMD_ELF

#define CONFIG_USB_GADGET
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_USB_GADGET_VBUS_DRAW 2

/* Netchip IDs */
#define CONFIG_G_DNL_VENDOR_NUM 0x0525
#define CONFIG_G_DNL_PRODUCT_NUM 0xa4a5
#define CONFIG_G_DNL_MANUFACTURER "Falinux"

#define CONFIG_CMD_FASTBOOT
#define CONFIG_ANDROID_BOOT_IMAGE
#define CONFIG_USB_FASTBOOT_BUF_ADDR   CONFIG_SYS_LOAD_ADDR
#define CONFIG_USB_FASTBOOT_BUF_SIZE   0x07000000

#endif         /* __CONFIG_H */

