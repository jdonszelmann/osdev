#include <stdio.h>
#include <interrupts.h>
#include <debug.h>

char *exception_messages[] = {
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

//int32_t thicc = 0;

void exceptionmaster_handler(uint32_t intid, registers_t * regs){
	if(!interrupt_exec_handler(intid,regs)){
		PANIC("Exception occured: %s", exception_messages[intid]);
		//printf("error %i\n", thicc++);
	}
}


//TODO clean up with macros

#define exceptionhandler(num) \
	void exception##num##_handler(registers_t * regs){ \
		exceptionmaster_handler(num,regs);\
	} 



exceptionhandler(0);//div by zero
exceptionhandler(1);
exceptionhandler(2);
exceptionhandler(3);
exceptionhandler(4);
exceptionhandler(5);
exceptionhandler(6);
exceptionhandler(7);
exceptionhandler(8); //double fault
exceptionhandler(9);
exceptionhandler(10); //bad TSS
exceptionhandler(11); //segment not present
exceptionhandler(12); //stack fault
exceptionhandler(13); //general protection fault
exceptionhandler(14); //page fault
exceptionhandler(15);
exceptionhandler(16);
exceptionhandler(17);
exceptionhandler(18);
exceptionhandler(19);
exceptionhandler(20);
exceptionhandler(21);
exceptionhandler(22);
exceptionhandler(23);
exceptionhandler(24);
exceptionhandler(25);
exceptionhandler(26);
exceptionhandler(27);
exceptionhandler(28);
exceptionhandler(29);
exceptionhandler(30);
exceptionhandler(31);

