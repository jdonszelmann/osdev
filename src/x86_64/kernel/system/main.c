
#include <multiboot2.h>
#include <bootconstants.h>
#include <kernel.h>
#include <system.h>
#include <timer.h>

bool divbyzero(){
	printf("division by zero");
	return true;
}

void kmain(void * multiboot_structure){
	UNUSED(multiboot_structure);
	system_init();
	int i = 0;
	printf("%i",1/i);
	
	printf("Hello, world!\n");

}