#include <kernel.h>

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

void exceptionmaster_handler(uint32_t intid){
	if(!interrupt_exec_handler(intid)){
		PANIC("Exception occured: %s", exception_messages[intid]);
		//printf("error %i\n", thicc++);
	}
}

//fatal means as of now, unrecoverable / BSOD. no recovery will be attempted.
void exceptionmaster_handler_fatal(uint32_t intid){
	PANIC("Fatal exception occured: %s", exception_messages[intid]);
}

//TODO clean up with macros

#define exceptionhandler(num) \
	void exception##num##_handler(){ \
		exceptionmaster_handler(num);\
	} 

#define exceptionhandler_fatal(num) \
	void exception##num##_handler(){ \
		exceptionmaster_handler_fatal(num);\
	} 


exceptionhandler(0);//div by zero
exceptionhandler(1);
exceptionhandler(2);
exceptionhandler(3);
exceptionhandler(4);
exceptionhandler(5);
exceptionhandler(6);
exceptionhandler(7);
exceptionhandler_fatal(8); //double fault
exceptionhandler(9);
exceptionhandler_fatal(10); //bad TSS
exceptionhandler_fatal(11); //segment not present
exceptionhandler_fatal(12); //stack fault
exceptionhandler_fatal(13); //general protection fault
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

