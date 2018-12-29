
#ifndef DRIVERS_H
#define DRIVERS_H
#include <kernel.h>

#define DRIVER_MAX 256

typedef bool (*driver_loader)();

void add_driver_loader(char* name, driver_loader loader);

void load_drivers();

#endif