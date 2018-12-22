

#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stdbool.h>


typedef struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry;


uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
bool initialize_gdt();
void load_gdt();

typedef struct gdtdescriptor {
	uint16_t size;
	uint32_t adress;
}__attribute__((packed)) gdtdescriptor;

#endif