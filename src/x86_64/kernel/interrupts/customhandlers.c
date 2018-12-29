

#include <stdio.h>
#include <interrupts.h>
#include <debug.h>

interrupthandler handlers[IDTSIZE] = {NULL};

//TODO find out why slow interrupt handlers cause double fault

bool interrupt_exec_handler(uint32_t intid, registers_t * regs){

	if(handlers[intid] != NULL){
		return handlers[intid](regs);
	}

	return false;
}

void interrupt_register_handler(uint32_t intid, interrupthandler handler){
	if(intid >= IDTSIZE){
		PANIC("trying to install interrupt handler; interrupt id invalid");
	}

	handlers[intid] = handler;

}

void interrupt_try_register_handler(uint32_t intid, interrupthandler handler){
	if(intid >= IDTSIZE){
		PANIC("trying to install interrupt handler; interrupt id invalid");
	}

	if(handlers[intid] == NULL){
		handlers[intid] = handler;
	}
}

void interrupt_deregister_handler(uint32_t intid){
	if(intid >= IDTSIZE){
		PANIC("trying to uninstall interrupt handler; interrupt id invalid");
	}
	handlers[intid] = NULL;
}

bool interrupt_check_handler(uint32_t intid){
	if(intid >= IDTSIZE){
		PANIC("trying to check interrupt handler; interrupt id invalid");
	}
	return handlers[intid] != NULL; 
}