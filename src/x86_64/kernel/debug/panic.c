
#include <vga.h>
#include <kernel.h>

void panic(char* file, uint32_t line,char* fmt, ...){
	asm volatile ("cli");
	va_list valist;
	va_start(valist,fmt);


	vga_setcolor(White,Red);
	//vga_cls();
	printf("\n");
	printf("PANIC! \n");
	vprintf(fmt, valist);
	printf("\nat ");
	printf(file);
	printf(":");
	printf("%i",line);
	printf("\n\n");


	printf("please reboot");  

	va_end(valist);
	
	for (;;);
}

void panic_assert(char* file, uint32_t line, char* fmt, ...){
	va_list valist;
	va_start(valist,fmt);


	asm volatile ("cli");
	vga_setcolor(White,Red);
	//vga_cls();

	printf("ASSERTION-FAILED(");
	vprintf(fmt, valist);
	printf(")\n at ");
	printf(file);
	printf(":");
	printf("%i",line);
	printf("\n\n");

	printf("please reboot");  

	va_end(valist);

	for (;;);
}