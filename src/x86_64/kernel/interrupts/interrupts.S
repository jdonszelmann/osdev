
#define ASM_FILE
#include <kernel.h>

.macro interrupt name number
	.extern \name\()_handler
	.global \name\()_handler
	.global \name
	\name:
		cli 
		push $0
		push $\number
		jmp irq_common
.endm

.macro exception name number
	.extern \name\()_handler
	.global \name\()_handler
	.global \name
	\name:
		cli 
		push $0
		push $\number
		jmp irq_common
.endm


exception e0 0
exception e1 1
exception e2 2
exception e3 3
exception e4 4
exception e5 5
exception e6 6
exception e7 7
exception e8 8
exception e9 9
exception e10 10
exception e11 11
exception e12 12
exception e13 13
exception e14 14
exception e15 15
exception e16 16
exception e17 17
exception e18 18
exception e19 19
exception e20 20
exception e21 21
exception e22 22
exception e23 23
exception e24 24
exception e25 25
exception e26 26
exception e27 27
exception e28 28
exception e29 29
exception e30 30
exception e31 31

interrupt irq0 32
interrupt irq1 33
interrupt irq2 34
interrupt irq3 35
interrupt irq4 36
interrupt irq5 37
interrupt irq6 38
interrupt irq7 39
interrupt irq8 40
interrupt irq9 41
interrupt irq10 42
interrupt irq11 43
interrupt irq12 44
interrupt irq13 45
interrupt irq14 46
interrupt irq15 47

exception_common:
		pusha
		push %ds
		push %es
		push %fs
		push %gs 
		mov $0x10, %ax
		mov %ax, %ds
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs
		mov %esp, %eax
		push %eax
		call exception_default_handler
		pop %eax
		pop %gs
		pop %fs
		pop %es
		pop %ds
		
		add %esp, 8
		
		popa
		iret

irq_common:
		pusha
		push %ds
		push %es
		push %fs
		push %gs 
		mov $0x10, %ax
		mov %ax, %ds
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs
		mov %esp, %eax
		push %eax
		call irq_default_handler
		pop %eax
		pop %gs
		pop %fs
		pop %es
		pop %ds
		
		add %esp, 8
		
		popa
		iret

.global load_idt
load_idt:
	cli
	movl 4(%esp), %edx
	lidt (%edx)
	sti
	ret