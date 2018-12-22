#include <stdio.h>

void NMI_enable() {
	outportb(0x70, inportb(0x70) & 0x7F);
}

void NMI_disable() {
	outportb(0x70, inportb(0x70) | 0x80);
}