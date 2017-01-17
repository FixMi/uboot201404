/*
 * Copyright (C) 2009 Samsung Electronics
 * Kyungmin Park <kyungmin.park@samsung.com>
 * Minkyu Kang <mk7.kang@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <version.h>
#include <asm/arch/cpu.h>
#include <asm/arch/power.h>
#include <asm/arch/tzpc.h>	/*add by Nick. */

/*
 * Register usages:
 *
 * r5 has zero always
 */

	.globl lowlevel_init
lowlevel_init:
	mov	r9, lr

#ifdef CONFIG_SPL_BUILD	
	bl system_clock_init		
    bl ddr_init			
		     
	/*add by Nick*/ 
    ldr r0, =0xE0200000		/* config GPIO as UART */
	ldr r1, =0x22222222
	str r1, [r0]
    /* TZPZ init. non-secure. add by Nick. */
    bl tzpc_init
			
#endif
	mov	pc, r9


#define	  APLL_CON0	   0xE0100100  @FOUT_APLL = 1000 MHZ
#define   APLL_VAL     ((1<<31)|(125<<16)|(3<<8)|(1<<0))    
	         
#define		MPLL_CON     0xE0100108  @FOUT_MPLL = 667 MHZ
#define   MPLL_VAL     ((1<<31)|(667<<16)|(12<<8)|(1<<0)) 

#define		EPLL_CON0    0xE0100110  @FOUT_EPLL = 96 MHZ
#define   EPLL_VAL     ((1<<31)|(48<<16)|(3<<8)|(2<<0)) 

#define		VPLL_CON	   0xE0100120  @FOUT_VPLL = 54 MHZ
#define   VPLL_VAL     ((1<<31)|(108<<16)|(6<<8)|(3<<0))


@ MOUT_MSYS = SCLKAPLL = FOUT_APLL = 1000MHz
@ MOUT_DSYS = SCLKMPLL = FOUT_MPLL = 667MHz
@ MOUT_PSYS = SCLKMPLL = FOUT_MPLL = 667MHz
@ ONENAND = HCLK_PSYS

#define   CLK_SRC0     0xE0100200
#define   SRC0_VAL     ((1<<0)|(1<<4)|(1<<8)|(1<<12))

@ APLL_RATIO = 0, freq(ARMCLK) = MOUT_MSYS / (APLL_RATIO + 1) = 1000MHz */
@ A2M_RATIO = 4, freq(A2M) = SCLKAPLL / (A2M_RATIO + 1) = 200MHz */
@ HCLK_MSYS_RATIO = 4, freq(HCLK_MSYS) = ARMCLK / (HCLK_MSYS_RATIO + 1) = 200MHz */
@ PCLK_MSYS_RATIO = 1, freq(PCLK_MSYS) = HCLK_MSYS / (PCLK_MSYS_RATIO + 1) = 100MHz */
@ HCLK_DSYS_RATIO = 3, freq(HCLK_DSYS) = MOUT_DSYS / (HCLK_DSYS_RATIO + 1) = 166MHz */
@ PCLK_DSYS_RATIO = 1, freq(PCLK_DSYS) = HCLK_DSYS / (PCLK_DSYS_RATIO + 1) = 83MHz */
@ HCLK_PSYS_RATIO = 4, freq(HCLK_PSYS) = MOUT_PSYS / (HCLK_PSYS_RATIO + 1) = 133MHz */
@ PCLK_PSYS_RATIO = 1, freq(PCLK_PSYS) = HCLK_PSYS / (PCLK_PSYS_RATIO + 1) = 66MHz */

#define   CLK_DIV0 0xE0100300
#define	  DIV0_VAL	((0<<0)|(4<<4)|(4<<8)|(1<<12)|(3<<16)|(1<<20)|(4<<24)|(1<<28))	


/*
    * system_clock_init: Initialize core clock and bus clock.
	 * void system_clock_init(void)
	  */
system_clock_init:
	ldr r0, =APLL_CON0
	ldr r1, =APLL_VAL
	str r1, [r0]
			
	ldr r0, =MPLL_CON
	ldr r1, =MPLL_VAL
	str r1, [r0]
					
	ldr r0, =EPLL_CON0 
	ldr r1, =EPLL_VAL
	str r1, [r0]
								
	ldr r0, =VPLL_CON
	ldr r1, =VPLL_VAL
	str r1, [r0]
									
	ldr r0, =CLK_SRC0
	ldr r1, =SRC0_VAL
	str r1, [r0]
															
	ldr r0, =CLK_DIV0
	ldr r1, =DIV0_VAL
	str r1, [r0]
													
	mov pc, lr


tzpc_init:

    ldr r0, =S5PV210_TZPC0_BASE
    mov r1, #0x0	
    str r1, [r0]  @ 0x00000000 = no secure region	
    mov r1, #0xff @ 1 = Decode region corresponding to the bit is non-secure
    str r1, [r0, #TZPC_DECPROT0SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT1SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT2SET_OFFSET]

    ldr     r0, =S5PV210_TZPC1_BASE
    str r1, [r0, #TZPC_DECPROT0SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT1SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT2SET_OFFSET]

    ldr r0, =S5PV210_TZPC2_BASE
    str r1, [r0, #TZPC_DECPROT0SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT1SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT2SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT3SET_OFFSET]

    ldr r0, =S5PV210_TZPC3_BASE
    str r1, [r0, #TZPC_DECPROT0SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT1SET_OFFSET]
    str r1, [r0, #TZPC_DECPROT2SET_OFFSET]

    mov pc, lr

