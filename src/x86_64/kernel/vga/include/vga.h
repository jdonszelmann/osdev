
#ifndef VGA_H
#define VGA_H

#include <vga_colors.h>
#include <stdio.h>
#include <stdint.h>

#define VGA_VIDEO_MEMORY ((uint16_t *)(0xB8000)) 


void vga_putc(char);

void vga_setbgcolor(vgacolor);
void vga_setfgcolor(vgacolor);
void vga_setcolor(vgacolor,vgacolor);
uint8_t vga_getcolorattr();
uint8_t vga_combinecolors(vgacolor,vgacolor);
void vga_resetcolors();


void vga_setx(uint32_t);
void vga_sety(uint32_t);
uint32_t vga_getx();
uint32_t vga_gety();

void vga_movecsr(uint32_t x, uint32_t y);

void vga_nextchar();
void vga_newline();
void vga_backspace();
void vga_rmline();
void vga_cls();

uint32_t vga_getvideomemoffset();
void vga_scroll(uint32_t);


uint32_t vga_xpos;
uint32_t vga_ypos;

const uint32_t vga_height;
const uint32_t vga_width;

uint8_t vga_lastcharofline[25];

vgacolor vga_bgcolor;
vgacolor vga_fgcolor;

#endif
