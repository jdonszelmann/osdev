
#include <kernel.h>

char * exception_messages[32] = {
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


//TODO add custom installable exception handlers 
void exception_handler(registers_t * reg) {
	if(reg->int_no < 32) {
    	irq_acknowledge(reg->int_no);
		PANIC("Unhandled exception: [%d] %s\n",reg->int_no, exception_messages[reg->int_no]);
	}else{
		irq_handler(reg);
	}
}