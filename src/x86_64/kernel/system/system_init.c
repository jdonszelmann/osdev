
#include <kernel.h>
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
	//allways first, else printing will triple fault
	kernel_set_printer(vga_putc);

	INIT_SYSTEM("gdt & tss",init_gdt);

	vga_cls();

	vga_resetcolors();
}	
