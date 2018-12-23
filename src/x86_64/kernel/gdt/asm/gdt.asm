
;TODO convert to att assembly

BITS 32
ALIGN 4
global gdt_flush
extern gdtpointer
gdt_flush:
	; Load the GDT
	lgdt [gdtpointer]
	; Flush the values to 0x10
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2
flush2:
	ret
