
#include <interrupts.h>
#include <gdt.h>
#include <stdio.h>
#include <vga.h>


#define INIT_SYSTEM(name,func) \
	vga_setfgcolor(Green); \
	printf("initializing %s...\n",name); \
	vga_setfgcolor(Yellow); \
	if(MACROCAT(func)()){\
		vga_setfgcolor(Green); \
		printf("%s initialized succesfully\n",name); \
	}else{ \
		PANIC("initializing " name " failed (func: " #func ")");\
	} \


void system_init(){
	INIT_SYSTEM("GDT",initialize_gdt);
	INIT_SYSTEM("PIC",initialize_pic);
	INIT_SYSTEM("IDT",initialize_interrupts);

	vga_resetcolors();
}