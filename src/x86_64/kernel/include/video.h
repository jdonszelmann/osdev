
#ifndef VIDEO_H
#define VIDEO_H


#include <vga.h>
#include <kernel.h>

#define videomode vga


#define kernel_putc(c) MACROCAT(videomode, _putc(c))
#define kernel_puts(c) MACROCAT(videomode, _puts(c))


#endif



