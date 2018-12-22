#ifndef PC_H
#define PC_H

#include <stdint.h>

void outportb(uint16_t port, uint8_t val);
uint8_t inportb(uint16_t port);
uint16_t inports(uint16_t _port);
void outports(uint16_t _port, uint16_t _data);
uint32_t inportl(uint16_t _port);
void outportl(uint16_t _port, uint32_t _data);


#define outportw(x,y) outports(x,y)
#define inportw(x) inports(x)

#endif