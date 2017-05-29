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
 /* #include <asm/arch/power.h> */
/*#include <asm/arch/tzpc.h> */	/*add by Nick. */

#ifdef CONFIG_EXYNOS4412
#include "tiny4412_val.h"
#else
#include "tiny4212_val.h"
#endif


/*
 * Register usages:
 *
 * r5 has zero always
 */

	.globl lowlevel_init
lowlevel_init:
	mov	r9, lr

#ifdef CONFIG_SPL_BUILD
	ldr	sp, =0x02060000

	/* set CP reset to low */
	ldr	r0, =0x11000C60
	ldr	r1, [r0]
	ldr	r2, =0xFFFFFF0F
	and	r1, r1, r2
	orr	r1, r1, #0x10
	str	r1, [r0]
	ldr	r0, =0x11000C68
	ldr	r1, [r0]
	ldr	r2, =0xFFFFFFF3
	and	r1, r1, r2
	orr	r1, r1, #0x4
	str	r1, [r0]
	ldr	r0, =0x11000C64
	ldr	r1, [r0]
	ldr	r2, =0xFFFFFFFD
	and	r1, r1, r2
	str	r1, [r0]

	/* led (GPM4_0~3) LED1 on */
	ldr	r0, =0x110002E0
	ldr	r1, =0x00001111
	str	r1, [r0]
	ldr	r1, =0x0e
	str	r1, [r0, #0x04]
		
	bl system_clock_init		
    
	bl  mem_ctrl_asm_init
 
	bl  uart_asm_init

 #if CONFIG_LL_DEBUG              
     mov r4, #0x4000
 .L0:
     sub r4, r4, #1
     cmp r4, #0
     bne .L0
 
     mov r0, #'\r'
     bl  uart_asm_putc
     mov r0, #'\n'
     bl  uart_asm_putc
 
     ldr r1, =0x40000000
     ldr r2, =0x87654321
     str r2, [r1]
     str r2, [r1, #0x04]
     str r2, [r1, #0x08]
     ldr r2, =0x55aaaa55
     str r2, [r1, #0x10]
     nop
 
     mov r4, #0xC0000
 .L1:
     subs    r4, r4, #1
     bne .L1
 
     ldr r0, [r1]
     bl  uart_asm_putx
     mov r0, #'.'
     bl  uart_asm_putc
 
     ldr r0, [r1, #0x04]
     bl  uart_asm_putx
     mov r0, #'.'
     bl  uart_asm_putc
 
     ldr r0, [r1, #0x08]
     bl  uart_asm_putx
     mov r0, #'.'
     bl  uart_asm_putc
 
     ldr r0, [r1, #0x10]
     bl  uart_asm_putx
     mov r0, #'>'
     bl  uart_asm_putc
 #endif /* CONFIG_LL_DEBUG */


	/* TZPZ init. non-secure. add by Nick. */
  //  bl tzpc_init

	/* set up C2C */
	ldr	r0, =EXYNOS4X12_SYSREG_BASE
	ldr	r2, =GENERAL_CTRL_C2C_OFFSET
	ldr	r1, [r0, r2]
	ldr	r3, =0x4000
	orr	r1, r1, r3
	str	r1, [r0, r2]
#else
	/* led (GPM4_0~3) LED2 on */
	ldr	r0, =0x110002E0
	ldr	r1, =0x00001111
	str	r1, [r0]
	ldr	r1, =0x0c
	str	r1, [r0, #0x04]			
#endif

	mov	pc, r9



#if 0
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
#endif


/*
 * uart_asm_init: Initialize UART in asm mode, 115200bps fixed.
 * void uart_asm_init(void)
 */
	.globl uart_asm_init
uart_asm_init:

	/* set GPIO to enable UART */
	@ GPIO setting for UART for UART0/1/2/3
	ldr	r0, =0x11400000
	ldr	r1, =0x22222222
	str	r1, [r0]
	ldr	r0, =0x11400020
	ldr	r1, =0x222222
	str	r1, [r0]

	ldr	r0, =EXYNOS4X12_CLOCK_BASE
	ldr	r1, =CLK_SRC_PERIL0_VAL
	ldr	r2, =CLK_SRC_PERIL0_OFFSET
	str	r1, [r0, r2]
	ldr	r1, =CLK_DIV_PERIL0_VAL
	ldr	r2, =CLK_DIV_PERIL0_OFFSET
	str	r1, [r0, r2]

	ldr	r0, =EXYNOS4X12_UART_CONSOLE_BASE
	ldr	r1, =0x111
	str	r1, [r0, #UFCON_OFFSET]

	mov	r1, #0x3
	str	r1, [r0, #ULCON_OFFSET]

	ldr	r1, =0x3c5
	str	r1, [r0, #UCON_OFFSET]

	ldr	r1, =UART_UBRDIV_VAL
	str	r1, [r0, #UBRDIV_OFFSET]

	ldr	r1, =UART_UDIVSLOT_VAL
	str	r1, [r0, #UDIVSLOT_OFFSET]

	ldr	r1, =0x4f4f4f4f
	str	r1, [r0, #UTXH_OFFSET]		@ print 'O'

	mov	pc, lr

#if CONFIG_LL_DEBUG
	.globl uart_asm_putc
uart_asm_putc:
	push	{r9}

	ldr	r9, =EXYNOS4X12_UART_CONSOLE_BASE
	str	r0, [r9, #UTXH_OFFSET]
	ldr	r9, =0x20000				@delay

.Luartputc:
	sub	r9, r9, #1
	cmp	r9, #0
	bne	.Luartputc

	pop	{r9}
	mov	pc, lr

	.globl uart_asm_putx
uart_asm_putx:
	stmfd sp!, {r3, r4, r5, lr}

	mov	r5, r0
	mov	r4, #28

.Luartputx:
	mov	r0, r5, asr r4
	and	r0, r0, #15
	cmp	r0, #9
	addle	r0, r0, #48
	addgt	r0, r0, #55
	bl	uart_asm_putc
	sub	r4, r4, #4
	cmn	r4, #4
	bne	.Luartputx

	ldmfd sp!, {r3, r4, r5, pc}

#endif /* CONFIG_LL_DEBUG */
