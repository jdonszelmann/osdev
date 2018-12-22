
#ifndef PRINTING_H
#define PRINTING_H

void kernel_putc(char);
void kernel_puts(char *);
void kernel_set_printer(void (*printer)(char));

void (*printerfunction)(char);

#endif