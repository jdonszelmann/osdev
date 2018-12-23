
;TODO convert to att assembly

BITS 32
ALIGN 4
global idt_load
extern idtpointer
idt_load:
	lidt [idtpointer]
	ret