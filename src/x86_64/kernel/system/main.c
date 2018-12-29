
#include <multiboot.h>
#include <bootconstants.h>
#include <debug.h>
#include <util.h>
#include <system.h>
#include <timer.h>
#include <IO.h>
#include <keyboard.h>
#include <serial.h>
#include <kheap.h>


void kmain(void *multiboot_structure)
{
	UNUSED(multiboot_structure);
	system_init();

	char * res = malloc(10*sizeof(char));

	res[0] = 'h';
	res[1] = 'e';
	res[2] = 'l';
	res[3] = 'l';
	res[4] = 'o';
	res[5] = '!';
	res[6] = '\n';
	res[7] = '\0';

	printf("ptr %d\n",(uint32_t)res);
	printf(res);
	free(res);
	print_kheap_map();


	for(;;){};
}