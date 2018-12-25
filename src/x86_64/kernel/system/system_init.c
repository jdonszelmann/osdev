
#include <kernel.h>
#include <serial.h>
#include <stdio.h>
#include <vga.h>
#include <drivers.h>
#include <timer.h>
#include <keyboard.h>
#include <rtc.h>

#define INIT_SYSTEM(name, func)                                  \
	vga_setfgcolor(Green);                                       \
	printf("initializing %s...\n", name);                        \
	vga_setfgcolor(Yellow);                                      \
	if(MACROCAT(func)()) {                                       \
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

	add_driver_loader("timer",init_timer);
	add_driver_loader("keyboard",init_keyboard);
	add_driver_loader("rtc", rtc_init);

	INIT_SYSTEM("idt", init_idt);

	//vga_cls();

	vga_resetcolors();
}
