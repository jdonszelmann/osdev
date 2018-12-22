
#include <printing.h>
#include <stdio.h>

void kernel_putc(char c){
	printerfunction(c);
}

void kernel_puts(char * str){
	for (char * c = str;*c!=NULL;c++){
		kernel_putc(*c);
	}

}

void kernel_set_printer(void (*printer)(char)){
	printerfunction = printer;
}