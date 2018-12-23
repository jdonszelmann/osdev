
#define ASM_FILE
#include <multiboot2.h>
#include <bootconstants.h>


.SET HEADER_LENGTH, multiboot_end - multiboot_start
.SET CHECKSUM, -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + HEADER_LENGTH)

.section .multiboot
multiboot_start:
	.long MULTIBOOT2_HEADER_MAGIC
	.long MULTIBOOT_ARCHITECTURE_I386
	.long HEADER_LENGTH
	.long CHECKSUM

	// multiboot tags go here

	.short MULTIBOOT_HEADER_TAG_END
	.short 0    // flags, none set
	.long 8     // size, including itself (short + short + long)
multiboot_end:


.section .stack

stack_bottom:
	.skip 16384 # 16 KiB
stack_top:

.section .bss


.section .text
.global start

start:
	mov $stack_top, %esp
	mov $stack_bottom, %ebp


	push %ebx
	call kmain

	cli
kernel_hlt:
	hlt
	jmp kernel_hlt
