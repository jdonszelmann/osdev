
#ifndef GDT_H
#define GDT_H

#include <kernel.h>

typedef struct gdt_entry{
	uint16_t limit_low;
	///Segment address
	uint16_t base_low;
	uint8_t base_middle;
	//Access modes
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
}gdt_entry_t;


//packed struct to represent a gdt pointer. 
//this is a 16 bit limit number followed by a 32 bit base address 
typedef struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_pointer_t;


typedef struct tss_entry {
	uintptr_t previous;
	uint32_t  esp0;
	uint32_t  ss0;
	uint32_t  esp1;
	uint32_t  ss1;
	uint32_t  esp2;
	uint32_t  ss2;
	uint32_t  cr3;
	uint32_t  eip;
	uint32_t  eflags;
	uint32_t  eax;
	uint32_t  ecx;
	uint32_t  edx;
	uint32_t  ebx;
	uint32_t  esp;
	uint32_t  ebp;
	uint32_t  esi;
	uint32_t  edi;
	uint32_t  es;
	uint32_t  cs;
	uint32_t  ss;
	uint32_t  ds;
	uint32_t  fs;
	uint32_t  gs;
	uint32_t  ldt;
	uint16_t  trap;
	uint16_t  iomap_base;
} __attribute__ ((packed)) tss_entry_t;


bool init_gdt();
void write_tss(int32_t, uint16_t, uint32_t);

tss_entry_t tss_entry;
gdt_entry_t gdt[6];
gdt_pointer_t gdtpointer;

void gdt_flush();
void tss_flush();

//TODO implement debug functions that print the current gdt and especially tss state transparancy is important i think

#endif




