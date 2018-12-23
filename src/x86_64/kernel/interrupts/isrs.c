
#include <kernel.h>

char * isrs_exception_messages[32] = {
	//0
	"Division by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	//5
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	//10
	"Bad TSS",
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	//15
	"Unknown interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

irq_handler_t isrs_routines[32] = { NULL };

void isrs_install_handler(uint32_t isrs,irq_handler_t handler) {
	isrs_routines[isrs] = handler;
}

void isrs_uninstall_handler(uint32_t isrs) {
	isrs_routines[isrs] = 0;
}

#define INSTALL_GATE(n) \
	void MACROCAT(_isr,n)(); \
	idt_set_gate(n, (uint32_t)MACROCAT(_isr,n), 0x08, 0x8E)


void isrs_install() {
	//DONT FOR LOOP THIS (preprocessor)
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
	INSTALL_GATE(16);
	INSTALL_GATE(17);
	INSTALL_GATE(18);
	INSTALL_GATE(19);
	INSTALL_GATE(20);
	INSTALL_GATE(21);
	INSTALL_GATE(22);
	INSTALL_GATE(23);
	INSTALL_GATE(24);
	INSTALL_GATE(25);
	INSTALL_GATE(26);
	INSTALL_GATE(27);
	INSTALL_GATE(28);
	INSTALL_GATE(29);
	INSTALL_GATE(30);
	INSTALL_GATE(31);


	// idt_set_gate(SYSCALL_VECTOR, (unsigned)_isr127, 0x08, 0x8E);
}



void fault_handler(registers_t * r) {
	printf("fault handler");
	// for(int i = 0;i < 1000000000;i++){
		// i = i;
	// }

	//double fault
	if (r->int_no == 8) {
		PANIC("DOUBLE FAULT");
	}
	//reserved exception
	if (r->int_no >= 32) {
		PANIC("RESERVED EXCEPTION #%i", r->int_no);
	}
	IRQ_OFF();
	irq_handler_t handler;
	handler = isrs_routines[r->int_no];
	if (handler) {
		handler(r);
	} else {
		PANIC("Unhandled exception: [%d] %s\n",r->int_no, isrs_exception_messages[r->int_no]);
	}
	IRQ_ON();
}