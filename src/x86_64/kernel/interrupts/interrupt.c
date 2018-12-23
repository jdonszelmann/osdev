#include <kernel.h>


irq_handler_t interrupt_handlers[256] = { NULL };

void register_interrupt_handler(uint32_t num, irq_handler_t handler){
	if(num < 256){
		interrupt_handlers[num] = handler;
	}
}

void deregister_interrupt_handler(uint32_t num) {
	if(num < 256){
		interrupt_handlers[num] = NULL;
	}
}

void irq_acknowledge(uint32_t irq_no) {
	outportb(0x20, 0x20);
	if (irq_no >= 12) {
		outportb(0xA0, 0x20);
	}
}

	
void irq_handler(registers_t * reg) {
	printf("an interrupt occured! %i\n",reg->int_no);

	irq_handler_t handler;
	if (r->int_no > 47 || r->int_no < 32) {
		handler = NULL;
	} else {
		handler = irq_routines[r->int_no - 32];
	}
	if (handler) {
		handler(r);
	} else {
		irq_acknowledge(r->int_no - 32);
	}
}
