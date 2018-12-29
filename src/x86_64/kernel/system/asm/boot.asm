


MBALIGN equ  1<<0
MEMINFO equ  1<<1
FLAGS equ  MBALIGN | MEMINFO
MAGIC equ  0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)


VM_BASE equ 0xC0000000
PDE_INDEX equ (VM_BASE >> 22)
PSE_BIT equ 0x00000010
PG_BIT equ 0x80000000

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .data

align 4096
global boot_page_directory
boot_page_directory:
	;identity map the first 4 megabytes
    dd 0x00000083
    times(PDE_INDEX - 1) dd 0
    dd 0x00000083
    times(1024 - PDE_INDEX - 1) dd 0 

;the kernel stack
section .stack, nobits
align 4
stack_bottom:
	;1 mb
    resb 104856
stack_top:

section .text

global start
start:
	;convert everything to virtual addresses, subtract the kernel base address
	mov ecx, (boot_page_directory - VM_BASE)
	mov cr3, ecx

	;use 4mb pages, so only one page has to be mapped for now
	mov ecx, cr4;
	or ecx, PSE_BIT
	mov cr4, ecx

	;start paging
	mov ecx, cr0
	or ecx, PG_BIT
	mov cr0, ecx

	;use absolute jump to avoid funny paging behaviour
	lea ecx, [higherhalf]
	jmp ecx


higherhalf:
    ;stop paging again. set it up later once a gdt is set up and we have interrupts
    mov dword[boot_page_directory], 0
    invlpg[0]

    mov esp, stack_top
    mov ebp, stack_bottom
    extern kmain

    ;multiboot struct
    push ebx
    call kmain

    ;stop interrupts
    sti
haltloop:
	hlt
	jmp haltloop