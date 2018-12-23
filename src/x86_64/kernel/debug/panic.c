
#include <kernel.h>
#include <vga.h>

//TODO convert vga_cls and vga_sectcolor to a generalized method or first make sure we are in vga mode.

void panic(char* message, char* file, uint32_t line){
	asm volatile ("cli");
	vga_setcolor(Red,Black);
	vga_cls();

	printf("PANIC! \n");
	printf(message);
	printf("\nat ");
	printf(file);
	printf(":");
	printf("%i",line);
	printf("\n\n");


	printf("please reboot");  

	for (;;);
}

// same as above but after assertion failed
void panic_assert(char* file, uint32_t line, char* desc){
	asm volatile ("cli");
	vga_setcolor(Red,Black);
	vga_cls();

	printf("ASSERTION-FAILED(");
	printf(desc);
	printf(")\n at ");
	printf(file);
	printf(":");
	printf("%i",line);
	printf("\n\n");

	printf("please reboot");  

	for (;;);
}