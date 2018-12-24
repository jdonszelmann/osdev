
#include <multiboot2.h>
#include <bootconstants.h>
#include <kernel.h>
#include <system.h>
#include <timer.h>
#include <IO.h>
#include <keyboard.h>

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

	while (true)
	{
		int8_t k = inportb(0x60);
		int8_t key[2] = {char_from_code(k), '\0'};
		printf("%s\n", key);
	}

	printf("Hello, world!\n");
}