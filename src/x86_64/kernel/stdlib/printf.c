
#include <stdio.h>

int printf(char *s, ...) {
	char buff[128];
	char *buffer = buff;

	va_list valist;
	va_start(valist, s);

	for (; *s != '\0'; s++) {
		if (*s == '\n') {
			kernel_putc('\n');
		} else if (*s != '%') {
			kernel_putc(*s);
		} else {
			s++;
			switch (*s) {
				case '\0':
					break; //Protection
				case 'd': //Signed decimal integer
				case 'i':
					itoa(va_arg(valist, int), buffer, 10);
					kernel_puts(buffer);
					break;
				case 's':
					kernel_puts(va_arg(valist, char *));
					break;
				case 'o': //Octal integer
					itoa(va_arg(valist, int), buffer, 8);
					kernel_puts(buffer);
					break;
				case 'x': //Hexadecimal integer
					itoa(va_arg(valist, int), buffer, 16);
					kernel_puts(buffer);
					break;
				case 'p':
					itoa((uint32_t)va_arg(valist, void *), buffer, 16);
					kernel_puts(buffer);
					break;
				case 'f':
					dtoa(va_arg(valist, double), buffer, -1);
					kernel_puts(buffer);
					break;
				case '%': //%
					kernel_puts("%%");
					break;
				default: //Anything else
					return 1;
			}
		}
	}

	va_end(valist);

	return 0;
}