
#ifndef SERIAL_H
#define SERIAL_H
#include <stdbool.h>

bool init_serial();

char read_serial();

void write_serial(char c);

void write_serial_str(char* s);

#endif