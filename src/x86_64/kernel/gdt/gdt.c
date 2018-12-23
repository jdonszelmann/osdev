
#include <kernel.h>

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	// Base Address 
	gdt[num].base_low =	(base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	// Limits 
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = (limit >> 16) & 0X0F;
	// Granularity 
	gdt[num].granularity |= (gran & 0xF0);
	// Access flags 
	gdt[num].access = access;
}

//TODO testing and return false if tests failed
bool init_gdt() {
	
	// GDT pointer and limits 
	gdtpointer.limit = (sizeof(gdt_entry_t) * 6) - 1;
	gdtpointer.base = (unsigned int)&gdt;
	// NULL 
	gdt_set_gate(0, 0, 0, 0, 0);
	// Code segment 
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	// Data segment 
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	// User code 
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	// User data 
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	write_tss(5, 0x10, 0x0);
	// Go go go 
	gdt_flush();
	tss_flush();

	return true;
}


void write_tss(int32_t num, uint16_t ss0, uint32_t esp0) {
	uintptr_t base  = (uintptr_t)&tss_entry;
	uintptr_t limit = base + sizeof(tss_entry);

	// Add the TSS descriptor to the GDT 
	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0x0, sizeof(tss_entry));

	tss_entry.ss0    = ss0;
	tss_entry.esp0   = esp0;
	// zeroing out but not with zeros just to confuse everybody... it's x86!
	tss_entry.cs = 0x0b;
	tss_entry.ss = 0x13;
	tss_entry.ds = 0x13;
	tss_entry.es = 0x13;
	tss_entry.fs = 0x13;
	tss_entry.gs = 0x13;
	tss_entry.iomap_base = sizeof(tss_entry);
}


void set_kernel_stack(uintptr_t stack) {
	tss_entry.esp0 = stack;
}
