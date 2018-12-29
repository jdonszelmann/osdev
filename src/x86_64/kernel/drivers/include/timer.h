
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <interrupts.h>
#include <IO.h>


bool init_timer();

uint64_t ms_since_bootup();

void sleep(uint64_t ms);

#endif