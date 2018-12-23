

#ifndef KERNEL_H
#define KERNEL_H

#define UNUSED(x) ((x)=(x))

#define MACROCAT(a, ...) PRIMITIVE_MACROCAT(a, __VA_ARGS__)
#define PRIMITIVE_MACROCAT(a, ...) a ## __VA_ARGS__

#ifndef ASM_FILE
#include <stdio.h>
#include <IO.h>
#include <debug.h>
#include <printing.h>
#include <gdt.h>
#include <interrupts.h>
#endif


#endif