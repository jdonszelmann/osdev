
#include <stdio.h>

#define ITOA_MAX_BUFFER 128

char *itoa(int32_t i, char *buffer, uint32_t base) {
	const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
	char pt[ITOA_MAX_BUFFER];
	char *p = pt;

	if (i < 0) {
		*p++ = '-';
		i *= -1;
	}

	int32_t shifter = i;

	do { //Find out the number of digits
		++p;
		shifter /= base;
	} while (shifter);

	*p = '\0';
	do { //Insert the digits int32_to the new string one by one
		*--p = digits[i%base];
		i = i / base;
	} while (i);

	return strcpy(buffer, p);
}

char *uitoa(uint32_t i, char *buffer, uint32_t base) {
	const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
	char pt[ITOA_MAX_BUFFER];
	char *p = pt;


	uint32_t shifter = i;

	do { //Find out the number of digits
		++p;
		shifter /= base;
	} while (shifter);

	*p = '\0';
	do { //Insert the digits int32_to the new string one by one
		*--p = digits[i%base];
		i = i / base;
	} while (i);

	return strcpy(buffer, p);
}