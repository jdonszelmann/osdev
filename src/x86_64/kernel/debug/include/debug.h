

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <interrupts.h>

struct registers;

void print_registers(struct registers * r);

#define PANIC(msg, args...) panic(__FILE__, __LINE__, msg, ##args)
#define ASSERT(b, args...) ((b) ? (void) 0 : panic_assert(__FILE__, __LINE__, #b, ##args))

void panic(char* file, uint32_t line,char* fmt, ...);
void panic_assert(char* file, uint32_t line, char* fmt, ...);

#endif