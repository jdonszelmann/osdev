#include <kernel.h>

#define INSTALL_GATE(n) \
	void MACROCAT(_irq,n)(); \
	idt_set_gate((n+32), (uint32_t)MACROCAT(_irq,n), 0x08, 0x8E)


irq_handler_t irq_routines[16] = { NULL };


//add an interrupt handler
void irq_install_handler(uint32_t irq,irq_handler_t handler) {
	irq_routines[irq] = handler;
}

//remove an interrupt handler
void irq_uninstall_handler(int irq) {
	irq_routines[irq] = NULL;
}

//remap interrupt handlers, see https://stackoverflow.com/questions/282983/setting-up-irq-mapping
void irq_remap() {
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);
}

void irq_gates() {
	INSTALL_GATE(0);
	INSTALL_GATE(1);
	INSTALL_GATE(2);
	INSTALL_GATE(3);
	INSTALL_GATE(4);
	INSTALL_GATE(5);
	INSTALL_GATE(6);
	INSTALL_GATE(7);
	INSTALL_GATE(8);
	INSTALL_GATE(9);
	INSTALL_GATE(10);
	INSTALL_GATE(11);
	INSTALL_GATE(12);
	INSTALL_GATE(13);
	INSTALL_GATE(14);
	INSTALL_GATE(15);
}


//TODO add checking and error handling/return false
bool init_irq() {
	irq_remap();
	irq_gates();
	IRQ_ON();
	return true;
}

void irq_acknowledge(int irq_no) {
	if (irq_no >= 12) {
		outportb(0xA0, 0x20);
	}
	outportb(0x20, 0x20);
}

void irq_handler(registers_t *r) {
	IRQ_OFF();
	irq_handler_t handler;
	if (r->int_no > 47 || r->int_no < 32) {
		handler = NULL;
	} else {
		handler = irq_routines[r->int_no - 32];
	}
	if (handler) {
		handler(r);
	} else {
		printf("int %i",r->int_no);
		irq_acknowledge(r->int_no - 32);
	}
	IRQ_ON();
}