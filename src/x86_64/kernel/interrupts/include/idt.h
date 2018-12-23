
#ifndef IDT_H
#define IDT_H

#include <kernel.h>

typedef struct idt_entry {
	unsigned short base_low;
	unsigned short sel;
	unsigned char zero;
	unsigned char flags;
	unsigned short base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
	unsigned short limit;
	uintptr_t base;
} __attribute__((packed)) idt_pointer_t;

idt_entry_t idt[256];
idt_pointer_t idtpointer;

void idt_load();
void idt_set_gate(uint8_t num,uint32_t base,uint16_t sel,uint8_t flags);
bool init_idt();

#endif