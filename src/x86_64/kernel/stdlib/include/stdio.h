
#ifndef STDIO_H
#define STDIO_H

#include <printing.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#define NULL 0

//printf.c
int printf(char * format, ...);
//itoa.c
char *itoa(int, char *, int);
//dtoa.c
char * dtoa(double f, char * buf, int precision);



#endif