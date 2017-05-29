/*
 * (C) Copyright 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * HeungJun Kim <riverful.kim@samsung.com>
 * Inki Dae <inki.dae@samsung.com>
 *
 * Configuation settings for the SAMSUNG SMDKC100 board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_SPL	/*add by Nick*/

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_SAMSUNG		1	/* in a SAMSUNG core */
#define CONFIG_S5P		1	/* which is in a S5P Family */
#define CONFIG_S5PC100		1	/* which is in a S5PC100 */
#define CONFIG_SMDKC220     1 
#define CONFIG_EXYNOS4412   1
#define CONFIG_TINY4412A	1


#define CONFIG_TRUSTZONE
#define CONFIG_TRUSTZONE_RESERVED_DRAM	0x100000

#define CONFIG_SECURE_BL1_ONLY


#include <asm/arch/cpu.h>		/* get chip and board defs */

#define CONFIG_ARCH_CPU_INIT

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/* input clock of PLL: TINY210 has 24MHz input clock */
#define CONFIG_SYS_CLK_FREQ		24000000
/* input clock of PLL: tiny210 has 24MHz input clock(add by Nick) */
#define CONFIG_SYS_CLK_FREQ_V210	24000000

/* DRAM Base */
#define CONFIG_SYS_SDRAM_BASE		0x40000000

/* Text Base */
#define CONFIG_SYS_TEXT_BASE		0x43E00000

#define CONFIG_SPL_TEXT_BASE	0x02023400

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE	/*add by Nick*/


/* APLL : 800MHz */
/*#define CONFIG_CLK_ARM_800_APLL_800*/
/* APLL : 1GHz */
/*#define CONFIG_CLK_ARM_1000_APLL_1000*/
/* APLL : 1.1GHz */
/*#define CONFIG_CLK_ARM_1200_APLL_1100*/
/* APLL : 1.2GHz */
/*#define CONFIG_CLK_ARM_1200_APLL_1200*/
/* APLL : 1.3GHz */
/*#define CONFIG_CLK_ARM_1200_APLL_1300*/
/* APLL : 1.4GHz */
#define CONFIG_CLK_ARM_1200_APLL_1400
/* APLL : 1.5GHz */
/*#define CONFIG_CLK_ARM_1500_APLL_1500*/

/* bus clock: 100Mhz, DMC clock 200Mhz */
/*#define CONFIG_CLK_BUS_DMC_100_200*/
/* bus clock: 165Mhz, DMC clock 330Mhz */
/*#define CONFIG_CLK_BUS_DMC_165_330*/
/* bus clock: 200Mhz, DMC clock 400Mhz */
#define CONFIG_CLK_BUS_DMC_200_400


/* IV_SIZE: 128 byte, 2 port(1 Gbyte), open page, trrd: 4 */
#define CONFIG_EVT0_PERFORMANCE
/* IV_SIZE: 512 Mbyte, 1 port(512 Mbyte), open page, trrd: 4 */
/*#define CONFIG_EVT0_STABLE*/
/* IV_SIZE: 128 byte, 2 port(1 Gbyte), close page, trrd: 0xA */
/*#define CONFIG_EVT0_RECOMMEND*/

/* (Memory Interleaving Size = 1 << IV_SIZE) */
#if defined(CONFIG_TINY4412A)
#define CONFIG_IV_SIZE 0x1F
#elif defined(CONFIG_EVT0_STABLE)
#define CONFIG_IV_SIZE 0x1D
#else
#define CONFIG_IV_SIZE 0x7
#endif

/* for DDR debug */
/*#define CONFIG_LL_DEBUG			1*/

#ifdef CONFIG_LL_DEBUG
#define CONFIG_RAM_TEST      1
#endif


/*
 * Size of malloc() pool
 * 1MB = 0x100000, 0x100000 = 1024 * 1024
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (1 << 20))
/*
 * select serial console configuration
 */
#define CONFIG_SERIAL0			1	/* use SERIAL 0 on SMDKC100 */

/* PWM */
#define CONFIG_PWM			1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200

/***********************************************************
 * Command definition
 ***********************************************************/
#include <config_cmd_default.h>

#undef CONFIG_CMD_FLASH
#undef CONFIG_CMD_IMLS
/*#undef CONFIG_CMD_NAND*/

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_REGINFO
/*modify by Nick*/
/*#define CONFIG_CMD_ONENAND*/
#define CONFIG_CMD_NAND
#define CONFIG_CMD_ELF
#define CONFIG_CMD_FAT
#define CONFIG_CMD_MTDPARTS

#define CONFIG_BOOTDELAY	1

#define CONFIG_ZERO_BOOTDELAY_CHECK

/*add by Nick*/
#define CONFIG_SYS_MAX_NAND_DEVICE		1
#define CONFIG_SYS_NAND_BASE			0xB0E00000
/*#define CONFIG_NAND_S5PV210				1*/
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_S5PV210_NAND_HWECC		1
#define CONFIG_SYS_NAND_ECCSIZE			512
#define CONFIG_SYS_NAND_ECCBYTES		13

/*add by Nick.*/
#define CONFIG_CMD_NAND_YAFFS			1


/*modify by Nick*/
#define MTDIDS_DEFAULT		"nand0=s5p-nand"
#define MTDPARTS_DEFAULT	"mtdparts=s5p-nand:1m(bootloader)"\
				",128k@0x100000(params)"\
				",5m@0x120000(kernel)"\
				",256m@0x620000(rootfs)"\
				",-(system)"

/*add by Nick.*/
#define IPADDR_DEFAULT		"192.168.0.102"
#define SERVERIP_DEFAULT	"192.168.0.100"
#define ETHADDR_DEFAULT		"12:34:56:78:AB:CD"
#define GATEWAYIP_DEFAULT	"192.168.0.1"
#define NETMASK_DEFAULT		"255.255.255.0"

#define NORMAL_MTDPARTS_DEFAULT MTDPARTS_DEFAULT

/* default flashboot (modify by Nick.)*/
#define CONFIG_BOOTCOMMAND	"run emmcboot"

#define CONFIG_RAMDISK_BOOT	"root=/dev/ram0 rw rootfstype=ext2" \
				" console=ttySAC0,115200n8" \
				" mem=128M"

#define CONFIG_COMMON_BOOT	"console=ttySAC0,115200n8" \
				" mem=512M " \
				" " MTDPARTS_DEFAULT

#define CONFIG_BOOTARGS	"noinitrd root=/dev/mtdblock3" \
			" rootfstype=yaffs2 rw " CONFIG_COMMON_BOOT

#define CONFIG_UPDATEB	"updateb=tftp 20000000 tiny210-u-boot.bin;nand erase.part bootloader;" \
			" nand write 20000000 bootloader $filesize\0"

#define CONFIG_ENV_OVERWRITE
#define CONFIG_EXTRA_ENV_SETTINGS					\
	CONFIG_UPDATEB \
	"updatek=" \
		"tftp 21000000 uImage;"\
		"nand erase.part kernel;" \
		"nand write 21000000 kernel $filesize\0" \
	"updateu=" \
		"tftp 20000000 tiny210-u-boot.bin;" \
		"nand erase.part bootloader;" \
		"nand write 20000000 bootloader $filesize\0" \
	"updatefs=" \
		"tftp 20000000 tiny210-rootfs.img;"\
		"nand erase.part rootfs;"\
		"nand write.yaffs 20000000 rootfs $filesize\0"\
	"bootk=" \
		"nand read 0x21000000 0x120000 0x300000;" \
		"bootm 0x21000000\0" \
	"flashboot=" \
		"set bootargs noinitrd root=/dev/mtdblock${bootblock} " \
		"rootfstype=${rootfstype} rw " CONFIG_COMMON_BOOT ";" \
		"run bootk\0" \
	"emmcboot=" \
		"set bootargs noinitrd root=/dev/mtdblock${bootblock} " \
		"rootfstype=${rootfstype} rw " CONFIG_COMMON_BOOT ";" \
		"\0" \
	"ubifsboot=" \
		"set bootargs root=ubi0!rootfs rootfstype=ubifs " \
		" ubi.mtd=${ubiblock} ${opts} " CONFIG_COMMON_BOOT "; " \
		"run bootk\0" \
	"boottrace=setenv opts initcall_debug; run bootcmd\0" \
	"android=" \
		"set bootargs root=ubi0!ramdisk ubi.mtd=${ubiblock} " \
		"rootfstype=ubifs init=/init.sh " CONFIG_COMMON_BOOT "; " \
		"run bootk\0" \
	"nfsboot=" \
		"set bootargs root=/dev/nfs " \
		"nfsroot=${nfsroot},nolock " \
		"ip=${ipaddr}:${serverip}:${gatewayip}:" \
		"${netmask}::eth0:off " CONFIG_COMMON_BOOT "; " \
		"run bootk\0" \
	"ramboot=" \
		"set bootargs " CONFIG_RAMDISK_BOOT \
		" initrd=0x33000000,8M ramdisk=8192\0" \
	"rootfstype=yaffs2\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"meminfo=mem=512M\0" \
	"nfsroot=/home/nick/nfs/rootfs\0" \
	"bootblock=3\0" \
	"ubiblock=4\0" \
	"ubi=disbled\0" \
	"ipaddr=" IPADDR_DEFAULT "\0" \
	"serverip=" SERVERIP_DEFAULT "\0" \
	"ethaddr=" ETHADDR_DEFAULT "\0" \
	"gatewayip=" GATEWAYIP_DEFAULT "\0" \
	"netmask=" NETMASK_DEFAULT

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser	*/
#define CONFIG_SYS_PROMPT		"TINY4412 # " /*modify by Nick*/
#define CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5e00000)
/*#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_MAPPED_RAM_BASE + 0x3e00000*/

/* SMDKC100 has 1 banks of DRAM, we use only one in U-Boot */
#ifdef CONFIG_EVT0_STABLE
#define CONFIG_NR_DRAM_BANKS	2
#else
#define CONFIG_NR_DRAM_BANKS	8
#endif
#define SDRAM_BANK_SIZE         0x08000000    /* 128 MB */
#define PHYS_SDRAM_1            CONFIG_SYS_SDRAM_BASE /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_2            (CONFIG_SYS_SDRAM_BASE + SDRAM_BANK_SIZE) /* SDRAM Bank #2 */
#define PHYS_SDRAM_2_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_3            (CONFIG_SYS_SDRAM_BASE + 2 * SDRAM_BANK_SIZE) /* SDRAM Bank #3 */
#define PHYS_SDRAM_3_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_4            (CONFIG_SYS_SDRAM_BASE + 3 * SDRAM_BANK_SIZE) /* SDRAM Bank #4 */
#define PHYS_SDRAM_4_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_5            (CONFIG_SYS_SDRAM_BASE + 4 * SDRAM_BANK_SIZE) /* SDRAM Bank #5 */
#define PHYS_SDRAM_5_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_6            (CONFIG_SYS_SDRAM_BASE + 5 * SDRAM_BANK_SIZE) /* SDRAM Bank #6 */
#define PHYS_SDRAM_6_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_7            (CONFIG_SYS_SDRAM_BASE + 6 * SDRAM_BANK_SIZE) /* SDRAM Bank #7 */
#define PHYS_SDRAM_7_SIZE       SDRAM_BANK_SIZE
#define PHYS_SDRAM_8            (CONFIG_SYS_SDRAM_BASE + 7 * SDRAM_BANK_SIZE) /* SDRAM Bank #8 */
#define PHYS_SDRAM_8_SIZE       SDRAM_BANK_SIZE

#define CONFIG_SYS_MONITOR_BASE	0x00000000

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_NO_FLASH		1

#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* 256 KiB */
#define CONFIG_IDENT_STRING		" for TINY210" /*modify by Nick*/

#if !defined(CONFIG_NAND_SPL) && (CONFIG_SYS_TEXT_BASE >= 0xc0000000)
/*#define CONFIG_ENABLE_MMU*/
#endif

#ifdef CONFIG_ENABLE_MMU
#define CONFIG_SYS_MAPPED_RAM_BASE	0xc0000000
#else
#define CONFIG_SYS_MAPPED_RAM_BASE	CONFIG_SYS_SDRAM_BASE
#endif

#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_MAPPED_RAM_BASE + 0x3e00000
#define CONFIG_PHY_UBOOT_BASE		CONFIG_SYS_SDRAM_BASE + 0x3e00000

/*-----------------------------------------------------------------------
 * Boot configuration
 */
 /*modify by Nick*/
/*#define CONFIG_ENV_IS_IN_ONENAND	1*/
#define CONFIG_ENV_IS_IN_NAND		1
#define CONFIG_ENV_SIZE			(128 << 10)	/* 128KiB, 0x20000 */
#define CONFIG_ENV_ADDR			(1 << 20)	/* 1MiB, 0x40000 */
#define CONFIG_ENV_OFFSET		(1 << 20)	/* 1MiB, 0x40000 */

#define CONFIG_USE_ONENAND_BOARD_INIT
/*#define CONFIG_SAMSUNG_ONENAND		1*/
#define CONFIG_SYS_ONENAND_BASE		0xE7100000

#define CONFIG_DOS_PARTITION		1

/*#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE + PHYS_SDRAM_1_SIZE * 4) */

#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_LOAD_ADDR - 0x1000000)

/*
 * Ethernet Contoller driver
 */
#ifdef CONFIG_CMD_NET
/*modify by Nick*/
#if 0
#define CONFIG_SMC911X         1       /* we have a SMC9115 on-board   */
#define CONFIG_SMC911X_16_BIT  1       /* SMC911X_16_BIT Mode          */
#define CONFIG_SMC911X_BASE    0x98800300      /* SMC911X Drive Base   */
#endif
/*modify by Nick*/
#define CONFIG_ENV_SROM_BANK   1       /* Select SROM Bank-3 for Ethernet*/
/*add by Nick*/
#define CONFIG_DRIVER_DM9000	1
#define CONFIG_DM9000_NO_SROM	1
#define CONFIG_DM9000_BASE		0x88000000
#define DM9000_IO				(CONFIG_DM9000_BASE)
#define DM9000_DATA				(CONFIG_DM9000_BASE + 0x4)
#define CONFIG_CMD_PING			
#endif /* CONFIG_CMD_NET */

#endif	/* __CONFIG_H */
