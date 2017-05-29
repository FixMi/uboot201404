/*
 *  Copyright (C) 2008-2009 Samsung Electronics
 *  Minkyu Kang <mk7.kang@samsung.com>
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/sromc.h>
#include <asm/arch/gpio.h>
#include <asm/arch/dmc.h>          /*add by Nick*/
//#include <asm/arch/nand_reg.h>	/* add by Nick */
#include <netdev.h>

#ifndef CONFIG_SPL_BUILD      /*add by Nick*/

DECLARE_GLOBAL_DATA_PTR;

static void dm9000_pre_init(void)
{
}


int board_init(void)
{

	gd->bd->bi_arch_number = MACH_TYPE_TINY4412;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((long *)PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE);

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
	gd->bd->bi_dram[2].start = PHYS_SDRAM_3;
	gd->bd->bi_dram[2].size = PHYS_SDRAM_3_SIZE;
	gd->bd->bi_dram[3].start = PHYS_SDRAM_4;
	gd->bd->bi_dram[3].size = PHYS_SDRAM_4_SIZE;	
	gd->bd->bi_dram[4].start = PHYS_SDRAM_5;
	gd->bd->bi_dram[4].size = PHYS_SDRAM_5_SIZE;
	gd->bd->bi_dram[5].start = PHYS_SDRAM_6;
	gd->bd->bi_dram[5].size = PHYS_SDRAM_6_SIZE;
	gd->bd->bi_dram[6].start = PHYS_SDRAM_7;
	gd->bd->bi_dram[6].size = PHYS_SDRAM_7_SIZE;
	gd->bd->bi_dram[7].start = PHYS_SDRAM_8;
	gd->bd->bi_dram[7].size = PHYS_SDRAM_8_SIZE;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("Board:\tTINY4412\n");
	return 0;
}
#endif

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, CONFIG_SMC911X_BASE);
/*add by Nick*/
#elif CONFIG_DRIVER_DM9000
	rc = dm9000_initialize(bis);
#endif
	return rc;
}

#else   /*CONFIG_SPL_BUILD  (add by Nick))*/
#define eFUSE_SIZE      (1 * 512)   // 512 Byte eFuse, 512 Byte reserved

#define MOVI_BLKSIZE        (1<<9) /* 512 bytes */

/* partition information */
#if defined(CONFIG_SECURE_BOOT) || defined(CONFIG_SECURE_BL1_ONLY)
#define PART_SIZE_FWBL1     (8 * 1024)
#else
#define PART_SIZE_FWBL1     0
#endif
#define PART_SIZE_BL1       (16 * 1024)                                                        
#ifdef CONFIG_TRUSTZONE
#define PART_SIZE_UBOOT     (328 * 1024)
#else
#define PART_SIZE_UBOOT     (512 * 1024)
#endif
#define PART_SIZE_KERNEL    (6 * 1024 * 1024)
#define PART_SIZE_ROOTFS    (26 * 1024 * 1024)
#define PART_SIZE_TZSW      (160 * 1024)

#define MOVI_FWBL1_BLKCNT   (PART_SIZE_FWBL1 / MOVI_BLKSIZE)
#define MOVI_BL1_BLKCNT     (PART_SIZE_BL1 / MOVI_BLKSIZE)
#define MOVI_ENV_BLKCNT     (CONFIG_ENV_SIZE / MOVI_BLKSIZE)    /* 16KB */
#define MOVI_UBOOT_BLKCNT   (PART_SIZE_UBOOT / MOVI_BLKSIZE)    /* 328KB */
#define MOVI_ZIMAGE_BLKCNT  (PART_SIZE_KERNEL / MOVI_BLKSIZE)   /* 6MB */
#define MOVI_ROOTFS_BLKCNT  (PART_SIZE_ROOTFS / MOVI_BLKSIZE)   /* 26MB */
#define MOVI_TZSW_BLKCNT    (PART_SIZE_TZSW / MOVI_BLKSIZE)     /* 160KB */

#define MOVI_UBOOT_POS      ((eFUSE_SIZE / MOVI_BLKSIZE) + MOVI_FWBL1_BLKCNT + MOVI_BL1_BLKCNT)


#define ISRAM_ADDRESS   0x02020000
#define SECURE_CONTEXT_BASE 0x02023000
#define EXTERNAL_FUNC_ADDRESS   (ISRAM_ADDRESS + 0x0030)
#define EXT_eMMC43_BL2_ByCPU_ADDRESS    (EXTERNAL_FUNC_ADDRESS + 0x4)
#define MSH_ReadFromFIFO_eMMC_ADDRESS   (EXTERNAL_FUNC_ADDRESS + 0x14)
#define MSH_EndBootOp_eMMC_ADDRESS  (EXTERNAL_FUNC_ADDRESS + 0x18)
#define LoadImageFromUsb_ADDRESS    (EXTERNAL_FUNC_ADDRESS + 0x40)


#define SDMMC_ReadBlocks_eMMC_ByCPU(uNumOfBlks, uDstAddr)   \
    (((void(*)(u32, u32*))(*((u32 *)EXT_eMMC43_BL2_ByCPU_ADDRESS)))(uNumOfBlks, uDstAddr))

#define SDMMC_ReadBlocks(uStartBlk, uNumOfBlks, uDstAddr)   \
    (((void(*)(u32, u32, u32*))(*((u32 *)EXTERNAL_FUNC_ADDRESS)))(uStartBlk, uNumOfBlks, uDstAddr))

#define LoadImageFromUsb()  \
    (((void(*)())(*((u32 *)LoadImageFromUsb_ADDRESS)))())


#ifdef CONFIG_RAM_TEST
extern void uart_asm_putc(int c);
extern void uart_asm_putx(int x);

#define MT_START	0x40000000
#define MT_LEN		0x10000000

void uboot_mem_test(void)
{
	unsigned int *p;
	int i;

	/* show dram config */
	p = (unsigned int *) 0x10600000;
	for (i = 0; i < 0x100/4; i++, p++) {
		if (!(i & 0xf)) {
			uart_asm_putc('\r');
			uart_asm_putc('\n');
		}
		uart_asm_putx(*p);
		uart_asm_putc('.');
	}
	p = (unsigned int *) 0x10610000;
	for (i = 0; i < 0x100/4; i++, p++) {
		if (!(i & 0xf)) {
			uart_asm_putc('\r');
			uart_asm_putc('\n');
		}
		uart_asm_putx(*p);
		uart_asm_putc('.');
	}

	uart_asm_putc('\r');
	uart_asm_putc('\n');

	uart_asm_putc('L');
	uart_asm_putc('e');
	uart_asm_putc('n');
	uart_asm_putc(':');
	uart_asm_putc(' ');
	uart_asm_putx(MT_LEN);

	uart_asm_putc('\r');
	uart_asm_putc('\n');
	uart_asm_putc('W');

	/* writing */
	p = (unsigned int *) MT_START;
	for (i = 0; i < MT_LEN/4; i++, p++) {
		*p = i+0x5a000000;
		if (!(i & 0xfffff)) {
			uart_asm_putc('.');
		}
	}

	uart_asm_putc('\r');
	uart_asm_putc('\n');
	uart_asm_putc('R');

	/* read and verify */
	p = (unsigned int *) MT_START;
	for (i = 0; i < MT_LEN/4; i++, p++) {
		if (*p != (i+0x5a000000)) {
			uart_asm_putc('X');
			uart_asm_putx(i);
			uart_asm_putx(p);
			if (i > 4) {
				uart_asm_putx(*(p-4));
				uart_asm_putx(*(p-3));
				uart_asm_putx(*(p-2));
				uart_asm_putx(*(p-1));
			}
			uart_asm_putx(*p);
			uart_asm_putx(*(p+1));
			uart_asm_putx(*(p+2));
			uart_asm_putx(*(p+3));
			uart_asm_putx(*(p+4));
			break;
		}

		if (!(i & 0xfffff)) {
			uart_asm_putc('.');
		}
	}

	uart_asm_putc('\r');
	uart_asm_putc('\n');
	uart_asm_putc('>');

#if 0
	/* dump memory */
	p = (unsigned int *) MT_START;
	for (i = 0; i < 0x200; i++, p++) {
		if (!(i & 0xf)) {
			uart_asm_putc('\r');
			uart_asm_putc('\n');
		}
		uart_asm_putx(*p);
		uart_asm_putc('.');
	}

	uart_asm_putc('\r');
	uart_asm_putc('\n');
#endif
}
#endif

void copy_bl2_to_ram(void)
{
	u32 OM = *(volatile u32 *)(0x10020000);	// OM Register
	OM &= 0x3e;

#ifdef CONFIG_RAM_TEST
	uboot_mem_test();
#endif				// 取低BIT[5:1]

	if (OM == 0x4) //sdcard boot
	{
		SDMMC_ReadBlocks(MOVI_UBOOT_POS, MOVI_UBOOT_BLKCNT, CONFIG_SYS_TEXT_BASE);
		//SDMMC_ReadBlocks(49, 1000, CONFIG_SYS_TEXT_BASE);			

#ifdef CONFIG_RAM_TEST
		{
			//test
			//dump UBOOT from DDR
			int i;
			unsigned int *p = (unsigned int  *) CONFIG_SYS_TEXT_BASE;

			uart_asm_putc('\r');
			uart_asm_putc('\n');
			
			for (i=0; i<128; i++)
			{

				if (i % 8 == 0)
				{
					uart_asm_putc('\r');
					uart_asm_putc('\n');
				}

				uart_asm_putx(*(p + i));
				uart_asm_putc(' ');

			}
			
			uart_asm_putc('\r');
			uart_asm_putc('\n');
		}
#endif
	}
	else
	{
	}



}
#endif
