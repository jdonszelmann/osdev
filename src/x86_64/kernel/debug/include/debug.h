

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <interrupts.h>

struct registers;

void print_registers(struct registers * r);

#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void) 0 : panic_assert(__FILE__, __LINE__, #b))

void panic(char* message, char* file, uint32_t line);
void panic_assert(char* file, uint32_t line, char* desc);

#endif