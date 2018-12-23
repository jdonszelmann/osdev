
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <idtdef.h>

typedef struct idt_entry {
	uint16_t base_low;
	uint16_t sel;
	uint8_t zero;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
	uint16_t limit;
	uintptr_t base;
} __attribute__((packed)) idt_pointer_t;


#define IRQ_OFF() { asm volatile ("cli"); }
#define IRQ_ON() { asm volatile ("sti"); }

typedef struct registers {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*irq_handler_t) (registers_t *);

void idt_load();
bool init_idt();

void deregister_interrupt_handler(uint32_t num);
void register_interrupt_handler(uint32_t num, irq_handler_t handler);

void irq_handler(registers_t * reg);
void exception_handler(registers_t * reg);

void irq_acknowledge(uint32_t irq_no);

#endif