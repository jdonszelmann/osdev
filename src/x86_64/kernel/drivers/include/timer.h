
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <interrupts.h>
#include <IO.h>


void init_timer(uint32_t freq);

uint64_t ms_since_bootup();

void sleep(uint64_t ms);

#endif