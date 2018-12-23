
#include <multiboot2.h>
#include <bootconstants.h>
#include <kernel.h>
#include <system.h>

void kmain(void * multiboot_structure){
	UNUSED(multiboot_structure);

	system_init();
	
	printf("Hello, world!\n");
	
	// PANIC("test %i", 10);

	asm volatile ("int $8");

	// int a = 0;
	// printf("%i\n",0/a);
}