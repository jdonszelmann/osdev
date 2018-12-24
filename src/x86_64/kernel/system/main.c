
#include <multiboot2.h>
#include <bootconstants.h>
#include <kernel.h>
#include <system.h>
#include <timer.h>
#include <IO.h>
#include <keyboard.h>
#include <serial.h>

bool divbyzero()
{
	printf("division by zero");
	return true;
}

void kmain(void *multiboot_structure)
{
	UNUSED(multiboot_structure);
	system_init();
	// int i = 0;
	// printf("%i",1/i);
	write_serial_str("hello!");
	printf("Hello, world!\n");

	for(;;){
		char str[2] = {read_serial(),'\0'};
		printf(str);
	};

}