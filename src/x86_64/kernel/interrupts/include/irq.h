
#ifndef IRQ_H
#define IRQ_H

//TODO chainable interrupts

#define IRQ_OFF() { asm volatile ("cli"); }
#define IRQ_ON() { asm volatile ("sti"); }

typedef struct registers {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*irq_handler_t) (registers_t *);

irq_handler_t irq_routines[16];

bool init_irq();
void irq_install_handler(uint32_t irq,irq_handler_t handler);
void irq_uninstall_handler(int irq);



#endif