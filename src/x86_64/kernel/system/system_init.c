
#include <stdio.h>
#include <serial.h>
#include <stdio.h>
#include <vga.h>
#include <interrupts.h>
#include <gdt.h>
#include <util.h>
#include <debug.h>
#include <memory.h>
#include <paging.h>


#define INIT_SYSTEM(name, func, args...)                         \
	vga_setfgcolor(Green);                                       \
	printf("initializing %s...\n", name);                        \
	vga_setfgcolor(Yellow);                                      \
	if(MACROCAT(func)(args)) {		                             \
		vga_setfgcolor(Green);                                   \
		printf("%s initialized succesfully\n", name);            \
	} else {                                                     \
		PANIC("initializing " name " failed (func: " #func ")"); \
	}

void system_init() {
	// allways first, else printing will triple fault
	kernel_set_printer(vga_putc);
	vga_cls();

	INIT_SYSTEM("serial", init_serial);
	INIT_SYSTEM("gdt & tss", init_gdt);
	INIT_SYSTEM("idt", init_idt);
	INIT_SYSTEM("memory", init_memory, 1024*1024*1024); //1GB for now. TODO: read memory map
	INIT_SYSTEM("paging", init_paging);
	INIT_SYSTEM("heap", init_kheap);

	//vga_cls();

	vga_resetcolors();
}
