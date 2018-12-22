
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

.align 4096
boot_page_directory:
	.skip 4096
boot_pagetable1:
	.skip 4096

.section .text
.global start

start:

	movl $(boot_pagetable1 - KERNEL_BASE_ADDR), %edi

	movl $0, %esi
	movl $1023, %ecx #map 1023 pages. 1024 is for the vga buffer

	map_kernel_pages_loop:
		cmpl $(KERNEL_VIRT_START - KERNEL_BASE_ADDR), %esi
		jl add_kernel_page_entry
		
		cmpl $(KERNEL_VIRT_END - KERNEL_BASE_ADDR), %esi
		jge map_kernel_pages_done

		#writable & present
		movl %esi, %edx
		orl $0x003, %edx
		movl %edx, (%edi)


		add_kernel_page_entry:

		addl $4096, %esi #1 page is 4096 bytes
		addl $4, %edi #1 page entry is 4 bytes
		loop map_kernel_pages_loop #until ecx = 0

	map_kernel_pages_done:

	#1 page for vga memory
	movl $(VGA_BUFFER_PHYS | 0x003), (boot_pagetable1 - KERNEL_BASE_ADDR + 1023 * 4)


	movl $(boot_pagetable1 - KERNEL_BASE_ADDR + 0x003), (boot_page_directory - KERNEL_BASE_ADDR + 0)
	movl $(boot_pagetable1 - KERNEL_BASE_ADDR + 0x003), (boot_page_directory - KERNEL_BASE_ADDR + 768 * 4)

	#load the page directory in cr3
	movl $(boot_page_directory - KERNEL_BASE_ADDR), %ecx
	movl %ecx, %cr3

	#enable paging
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0		

	#jump to paging
	lea paging_start, %ecx
	jmp *%ecx


paging_start:
	movl $0, boot_page_directory + 0

	# Reload cr3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	mov $stack_top, %esp
	mov $stack_bottom, %ebp

	addl $KERNEL_BASE_ADDR, %ebx
	push %ebx
	call kmain

	cli
kernel_hlt:
	hlt
	jmp kernel_hlt
