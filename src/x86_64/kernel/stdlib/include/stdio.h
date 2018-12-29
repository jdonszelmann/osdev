
#ifndef STDIO_H
#define STDIO_H

#include <printing.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#define NULL 0

//printf.c
int printf(char * format, ...);
int vprintf(char * format, va_list valist);
int vsprintf(char * res, char *fmt, va_list valist);
int sprintf(char * res, char *fmt, ...);


//itoa.c
char *itoa(int32_t, char *, uint32_t);
char *uitoa(uint32_t, char *, uint32_t);
//dtoa.c
char * dtoa(double f, char * buf, int32_t precision);



#endif