
#ifndef ISRS_H
#define ISRS_H


void isrs_install_handler(uint32_t isrs,irq_handler_t handler);
void isrs_uninstall_handler(uint32_t isrs);

#endif