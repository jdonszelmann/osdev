
#include <multiboot2.h>
#include <kernel.h>

void kmain(void * multiboot_structure){
	UNUSED(multiboot_structure);


	*((int*)0xb8000)=0x07690748;

}