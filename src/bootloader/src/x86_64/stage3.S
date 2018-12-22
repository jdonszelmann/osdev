
#include <boot.h>

.section .stage3.data

.section .stage3
.code64
.global _cont64long

_cont64long:
	cli								# Clear the interrupt flag. TODO superfluos?
	mov		$gdt_data_segment, %ax	# Set the A-register to the data descriptor.
	mov		%ax, %fs				# Set the F-segment to the data descriptor
	mov		%ax, %gs				# Set the G-segment to the data descriptor
									# NB: ds, es, not used in 64 bit mode; setting them anyway to all be the same:

	mov		%ax, %ds				# Set the D-segment to the data descriptor
	mov		%ax, %es				# Set the E-segment to the data descriptor

	mov		%ax, %ss				# setting SS; said to be unused, but seems used with interrupt handling anyway
	and		$-16, %rsp				# ands RSP with 0xFFF...FFF0, aligning the stack with the next lowest 16-byte boundary. TODO needed?


# load the kernel from disk

# Time to start loading a kernel or something
	jmp		0x10000;				# that's where we hard code our kernel starts

# The end
halt:
	hlt								# Halt the processor.
	jmp	halt

