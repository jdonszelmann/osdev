;TODO to at&t syntax

global load_idt


 
%macro IRQ 2
    global irq%1
    extern irq%1_handler
irq%1:
    cli
    push byte 0 ; error code is allways-0 for irqs
    push byte %2

    pusha

    mov ax, ds
    push eax

    mov ax, 0x10; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp

    call irq%1_handler

    pop esp

    pop ebx; restore original data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    add esp, 0x08

    sti
    iret
%endmacro

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	ret