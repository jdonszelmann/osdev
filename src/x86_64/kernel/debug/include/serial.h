
#ifndef SERIAL_H
#define SERIAL_H
#include <stdio.h>

bool init_serial();
char read_serial();
void write_serial(char c);
void write_serial_str(char* s);
void printf_serial(char * fmt, ...);
void seriallog(uint32_t line, char * file, char * fmt, ...);

#define LOG(msg, args...) seriallog(__LINE__, __FILE__, msg, ##args)


#endif