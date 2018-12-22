
#include <boot.h>

# i think the a:b references are intel manual references
# TODO cache is disabled; see 2:431
# TODO mtrr is disabeld? 2:431
# TODO bios printf for debug messages 
.code16


.section .stage1.data
GDT32info:
			.word	(gdt32_end - gdt32 - 1)	# n * 8 - 1
			.long	gdt32 # to get the lineair address
gdt32:
			.quad	0x0 # entry 0: unused
			.quad	0x00CF9A000000FFFF # entry 1: all 4 GB is kernel code
			.quad	0x00CF92000000FFFF # entry 2: all 4 GB is kernel data
			# TODO: a stack segment?
gdt32_end:
boot_disk: .byte 0x42

bootmsg: .asciz "boot succesfull                            \r\n"
badload: .asciz "error loading kernel from disk \r\n"
oknow: .asciz "still ok \r\n"
okload: .asciz "succesfully loaded kernel from disk"

.section .stage1
.code16

.global _start
_start:
	#disable interrupts
	cli
	#use longjmp to fix the code segment register
	ljmp $0x0, $realstart
realstart:
	#setup string operations
	cld
	#set he data segment to the same as the code segment
	mov %cs, %bx 
	mov %bx, %ds
	#setup stack (including stack segment because fuck segmentation) 
	mov $bootaddr, %ebx
	lss (%ebx), %sp

	call bios_cls
	mov $bootmsg, %si
	call bios_puts

	# eax contains BIST. save to maybe use later
	push %eax
	#%dl is the bootdrive
	mov %dl, boot_disk #harddisk 0
	movzb boot_disk, %ax
	push %ax

	#read disk info
	#ask bios for CHS (cylinder, head, sector)
	mov $8, %ah
	#disk number
	mov boot_disk, %dl
	int $0x13

	#dh is one less than the ammount of heads
	mov %dh, %al
	and $0xFF, %ax
	inc %ax

	#cl is the ammount of sectors per track
	and $0x3F, %cx

	#read the next sectors that contain stage 2
	mov $bootaddr2, %ax
	mov %ax, %es
	mov $0, %bx
	mov $2, %ah	# read (see bios calls, see link below)
	mov $4, %al # number of sectors to read
	mov $0, %ch # cylinder number
	mov $2, %cl # sector number
	mov $0, %dh # head number
	mov boot_disk, %dl # disk to read from
	int $0x13 # https://en.wikipedia.org/wiki/BIOS_interrupt_call
	cmp $0, %ah # check result code

	# read the next sectors that contain the kernel
	# reads go to es:bx, so set those first:
	mov $kerneladdr, %ax
	mov %ax, %es
	mov $0, %bx
	mov $2, %ah # read
	mov $128, %al # number of sectors to read. if kernel is larger, increment this number
	mov $0, %ch	# cylinder number
	mov $18, %cl # sector number, 18 is where kernel starts
	mov $0, %dh	# head number
	mov boot_disk, %dl # disk to read from
	int $0x13 # https://en.wikipedia.org/wiki/BIOS_interrupt_call

	// mov $oknow, %si
	// call bios_puts

	cmp $0, %ah # check result code
	jmp part2ok #start bootloader stage 2

	#an error occured, halt
	
	mov $badload, %si
	call bios_puts

	stage1hlt:
		hlt
		jmp stage1hlt

	part2ok:
		mov $okload, %si
		call bios_puts
		jmp parttwo16

bios_puts:
	pusha
	mov $0x0e, %ah
	bios_puts_loop:
		lodsb
		or %al, %al
		jz bios_puts_done
		int $0x10
		jmp bios_puts_loop
	bios_puts_done:
	popa
	ret

bios_cls:
	pusha
	mov $0x00, %ah
	mov $0x03, %al
	int $0x10
	popa
	ret

.section .stage12.data

startstage12: .asciz "entered bootloader stage 1.2"

.section .stage12
parttwo16:

	mov $startstage12, %si
	call bios_puts


	call testA20
	cmp $0, %ax
	je A20off
	jmp A20on
A20off:
	# A20 gate was off, enable
	mov $0x2401, %ax
	int $0x15
A20on:	
	#A20 is certainly on here
	
	# read available memory #TODO expand memory map? c readable struct maybe
	call getMemoryMap

	# load the GDT
	lgdt GDT32info

	#enable protected mode
	mov %cr0, %eax
	or $1, %eax
	mov %eax, %cr0

	# far jump to protected code
	ljmp $gdt_code_segment, $stage2start

	#when unsuccesful or kernel returned, halt
	jmp stage1hlt



#retrieves the memory map
#first the low mem part, then the complete map
getMemoryMap:
	mov $0, %ax
	int $0x12 # request low memory size
	jc getMemoryMap_exitCF
	test %ax, %ax
	jz getMemoryMap_exitCF

	# start of E820 calls
	mov $0, %ebx
	mov %bx, %es
	# store mem map at the beginning of usable ram, first 2 bytes will be count
	mov $0x502, %di
	# entry count
	mov $0, %bp
	# "SMAP" TODO: what's smap?
	mov $0x0534D4150, %edx
	mov $0xE820, %eax
	# ask for 24 bytes
	mov $24, %ecx
	int $0x15
	jc getMemoryMap_exitCF
	# "SMAP"; restore TODO needed?
	mov $0x0534D4150, %edx
	# eax should be SMAP now
	cmp %edx, %eax
	jne getMemoryMap_exitCF
	# no entries? Something failed
	test %ebx, %ebx
	je getMemoryMap_exitCF
	jmp getMemoryMap_loopentry
getMemoryMap_loop:
	# gets trashed on every int 0x15 call; put it back
	mov $0xE820, %eax
	# is this needed? TODO
	mov $24, %ecx
	int $0x15
	jc getMemoryMap_loopbreak
	# "SMAP"; restore edx
	mov $0x0534D4150, %edx
getMemoryMap_loopentry:
	# skip 0 byte entries
	jcxz getMemoryMap_loopcontinue
	# got a 24 byte ACPI 3.X response?
	cmp $20, %cl
	jbe getMemoryMap_noresp
	# is the ignore bit clear?
	testb $1, %es:20(%di)
	je getMemoryMap_loopcontinue
getMemoryMap_noresp:
	mov %es:8(%di), %ecx # lower word of memory region length
	or %es:12(%di), %ecx # upper part
	jz getMemoryMap_loopcontinue # if both or-ed is zero, length is zero
	inc %bp	# count++
	# next entry 24 bytes further
	add $24, %di
getMemoryMap_loopcontinue:
	test %ebx, %ebx
	#when ebx is zero, exit (which is the same as not jumping to loop)
	jne getMemoryMap_loop
getMemoryMap_loopbreak:
	mov $0x500, %di
	mov %bp, (%di) # store the count before the content

	clc # TODO why clear carry flag? comment
	jmp getMemoryMap_exit
getMemoryMap_exitCF:
	stc # TODO why carry flag?
getMemoryMap_exit:
	ret


# Function: check_a20
# 
# Purpose: 
# 	to check the status of the a20 line in a completely self-contained state-preserving way.
# 	The function can be modified as necessary by removing push's at the beginning and their
# 	respective pop's at the end if complete self-containment is not required.
# 
# Returns: 
# 	0 in ax if the a20 line is disabled (memory wraps around)
# 	1 in ax if the a20 line is enabled (memory does not wrap around)
  
#TODO: comment this. right now it's completely ununderstandable
testA20:
	#save register state
	enter $0, $0
	push %ds
	pushf


	mov $0xFFFF, %ax # can be removed?

	mov $0x0500, %di
	mov $0x0510, %si

	mov $0, %bx
	mov %bx, %es
	mov $0xFFFF, %bx
	mov %bx, %ds
	mov %es:(%di), %al
	push %ax

	mov %ds:(%si), %al
	push %ax

	movb $0x00, %es:(%di)
	movb $0xFF, %ds:(%si)

	cmpb $0xFF, %es:(%di)

	pop %ax
	mov %al, %ds:(%si)

	pop %ax
	mov %al, %es:(%di)

	#return boolean
	mov $0, %ax
	je testA20_exit

	mov $1, %ax

testA20_exit:
	#clean up used registers

	popf
	pop %ds
	
	leave
	ret

