
#include <multiboot.h>
#include <bootconstants.h>
#include <debug.h>
#include <util.h>
#include <system.h>
#include <timer.h>
#include <IO.h>
#include <keyboard.h>
#include <multiboot2.h>
#include <rtc.h>
#include <serial.h>
#include <kheap.h>
#include <system.h>
#include <timer.h>


void kmain(void *multiboot_structure) {
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


	// int i = 0;
	// printf("%i",1/i);
	write_serial_str("hello!\n");
	printf("Hello, world!\n");

	printf("test\n");
	sleep(1000);
	printf("after 1 ms\n");
	rtc_hour_offset(1);
	// while(true) {
	// 	datetime_t dt = get_datetime();

	// 	printf("%i-%i-%i %i:%i:%i\n", dt.year, dt.month, dt.day, dt.hour,
	// 		   dt.minute, dt.second);
	// 	sleep(1000);
	// }

	// clang-format off
	for(;;);
	// clang-format on

}