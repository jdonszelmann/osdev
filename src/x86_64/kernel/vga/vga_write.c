
#include <constants.h>
#include <vga.h>
#include <stdio.h>


void vga_putc(char c){

	if(c == '\n'){
		vga_newline();
		return;
	}
	if(c == '\b'){
		vga_backspace();
		return;
	}
	if(c == '\r'){
		vga_rmline();
		return;
	}

	VGA_VIDEO_MEMORY[vga_getvideomemoffset()] = c | vga_getcolorattr() << 8;
	vga_nextchar();
}



