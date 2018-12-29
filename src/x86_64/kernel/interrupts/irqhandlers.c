#include <interrupts.h>
#include <stdio.h>
#include <IO.h>

void irqmaster_handler(uint32_t intid, registers_t * regs) { 
	interrupt_exec_handler(intid + 32, regs); 
}

// TODO clean up with macros

//TODO move this to time.c plz
int64_t time = 0;
// timer
void irq0_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(0, regs);
}

// kbd
void irq1_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI0x64
	irqmaster_handler(1, regs);
}

void irq2_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(2, regs);
}

void irq3_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(3, regs);
}

void irq4_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(4, regs);
}

void irq5_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(5, regs);
}

void irq6_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(6, regs);
}

void irq7_handler(registers_t * regs) {
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(7, regs);
}

void irq8_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(8, regs);
}

void irq9_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(9, regs);
}

void irq10_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(10, regs);
}

void irq11_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(11, regs);
}

void irq12_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(12, regs);
}

void irq13_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(13, regs);
}

void irq14_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(14, regs);
}

void irq15_handler(registers_t * regs) {
	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // EOI
	irqmaster_handler(15, regs);
}
