#include <interrupts.h>
#include <kernel.h>
#include <debug.h>

void * irq_routines[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void irq_default_handler(registers * r){

	printf("IRQ %i occured\n",r->int_no);
	print_registers(r);

	void (*handler)(struct registers *r);

	handler = irq_routines[r->int_no - 32];

	if (handler){
		handler(r);
	}

	if (r->int_no >= 40){
		outportb(0xA0, 0x20);
	}
	outportb(0x20, 0x20);

}


typedef void (*irqhandler)(registers * r);

void irq_install_handler(int irq, irqhandler handler){
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq){
    irq_routines[irq] = 0;
}