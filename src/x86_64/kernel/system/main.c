
#include <IO.h>
#include <bootconstants.h>
#include <kernel.h>
#include <keyboard.h>
#include <multiboot2.h>
#include <serial.h>
#include <system.h>
#include <timer.h>

bool divbyzero() {
	printf("division by zero");
	return true;
}

void kmain(void *multiboot_structure) {
	UNUSED(multiboot_structure);
	system_init();
	// int i = 0;
	// printf("%i",1/i);
	write_serial_str("hello!\n");
	printf("Hello, world!\n");

	printf("test\n");
	sleep(1000);
	printf("after 1 ms");

	// clang-format off
	for(;;);
	// clang-format on
}