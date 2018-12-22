
#include <vga.h>
#include <stdint.h>

uint8_t vga_combinecolors(vgacolor fg,vgacolor bg){
	return (bg << 4) | (fg & 0x0F);
}


void vga_setbgcolor(vgacolor bg){
	vga_bgcolor = bg;
}

void vga_setfgcolor(vgacolor fg){
	vga_fgcolor = fg;
}

void vga_setcolor(vgacolor fg,vgacolor bg){
	vga_setfgcolor(fg);
	vga_setbgcolor(bg);
}

uint8_t vga_getcolorattr(){
	return vga_combinecolors(vga_fgcolor,vga_bgcolor);
}

void vga_resetcolors(){
	vga_setbgcolor(Black);
	vga_setfgcolor(White);
}