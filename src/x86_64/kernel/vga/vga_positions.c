

#include <stdio.h>
#include <vga.h>
#include <IO.h>


void vga_setx(uint32_t x){
	vga_xpos = x;
}

void vga_sety(uint32_t y){
	vga_ypos = y;
}

uint32_t vga_getx(){
	return vga_xpos;
}

uint32_t vga_gety(){
	return vga_ypos;
}

uint32_t vga_getvideomemoffset(){
	return vga_xpos + vga_ypos*vga_width;
}

void vga_scroll(uint32_t ammount){

	uint32_t blank, temp;

	blank = 0x20 | (vga_getcolorattr() << 8);

	for (uint32_t i = 0; i < ammount; ++i){
		temp = vga_ypos - vga_height + 1;
		memcpy (VGA_VIDEO_MEMORY, VGA_VIDEO_MEMORY + temp * vga_width, (vga_height - temp) * vga_width * 2);
		memsetw (VGA_VIDEO_MEMORY + (vga_height - temp) * vga_width, blank, vga_width);
		vga_ypos = vga_height - 1;
	}
}

void vga_nextchar(){
	vga_xpos++;
	if(vga_xpos > vga_width){
		vga_newline();
	}
	vga_movecsr(vga_xpos,vga_ypos);
}

void vga_newline(){
	
	vga_lastcharofline[vga_ypos] = vga_xpos;

	vga_xpos=0;
	vga_ypos++;
	vga_movecsr(vga_xpos,vga_ypos);
	if(vga_ypos == vga_height){
		vga_scroll(1);
	}
}

void vga_backspace(){
	if(vga_xpos != 0){
		vga_xpos--;
		vga_putc(' ');
		vga_xpos--;
	}else if(vga_ypos != 0){
		vga_ypos--;
		vga_xpos = vga_lastcharofline[vga_ypos];
	}else{
		//do nothing
	}
	vga_movecsr(vga_xpos,vga_ypos);
}

void vga_rmline(){
	uint32_t tempypos = vga_ypos;
	while(vga_ypos == tempypos){
		vga_backspace();
	}
}

void vga_movecsr(uint32_t x, uint32_t y){

	uint32_t position = (y * vga_width) + x;

	// This sends a command to indicies 14 and 15 in the
	// CRT Control Register of the VGA controller. These
	// are the high and low bytes of the index that show
	// where the hardware cursor is to be 'blinking'.
	outportb(0x3D4, 14);
	outportb(0x3D5, position >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, position);
}

void vga_cls(){
	memsetw((uint16_t *)VGA_VIDEO_MEMORY,0x20 | vga_getcolorattr()>>8,80*25);
    vga_xpos = 0;
    vga_ypos = 0;
    vga_movecsr(0,0);
}