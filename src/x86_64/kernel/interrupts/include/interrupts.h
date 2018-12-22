

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdio.h>
#include <stdbool.h>
#include <IO.h>


typedef struct idt_entry{
	uint16_t offset_lower;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higher;
}__attribute__((packed)) idt_entry;
 
idt_entry idt[256];

typedef struct registers{
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
}registers;


typedef struct idtdescriptor {
	uint16_t size;
	uint32_t adress;
}__attribute__((packed)) idtdescriptor;


bool initialize_interrupts();
bool initialize_pic();

void NMI_enable();
void NMI_disable();

#define endofinterrupt() outportb(0x20, 0x20)

#endif