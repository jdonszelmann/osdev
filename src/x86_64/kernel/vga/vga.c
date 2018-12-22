

#include <vga.h>
#include <stdio.h>


uint32_t vga_xpos = 0;
uint32_t vga_ypos = 0;
const uint32_t vga_height = 25;
const uint32_t vga_width = 80;
vgacolor vga_bgcolor = Black;
vgacolor vga_fgcolor = White;

uint8_t vga_lastcharofline[25] = {0};
