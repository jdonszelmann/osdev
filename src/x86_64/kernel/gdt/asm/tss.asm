
;TODO convert to att assembly

BITS 32
ALIGN 4

global tss_flush
tss_flush:
	mov ax, 0x2B
	ltr ax
	ret