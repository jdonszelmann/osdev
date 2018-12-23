
#include <multiboot2.h>
#include <bootconstants.h>
#include <kernel.h>
#include <system.h>


bool divbyzero(){
	printf("division by zero");
	return true;
}

void kmain(void * multiboot_structure){
	UNUSED(multiboot_structure);
	system_init();


	
	printf("Hello, world!\n");

}