
;TODO convert to att assembly

BITS 32
global idt_load
idt_load:
	mov eax, [esp + 4]
	lidt [eax]
	ret