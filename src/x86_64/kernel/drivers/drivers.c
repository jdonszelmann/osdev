
#include <drivers.h>

driver_loader loaders[DRIVER_MAX];
char* drivernames[DRIVER_MAX];
uint8_t counter = 0;

void add_driver_loader(char* name, driver_loader loader) {
	drivernames[counter] = name;
	loaders[counter++] = loader;
}

void load_drivers() {
	for(uint8_t i = 0; i < counter; i++) {
		if(!loaders[i]()) {
			PANIC("Error loading driver %s!", drivernames[i]);
		}
	}
}