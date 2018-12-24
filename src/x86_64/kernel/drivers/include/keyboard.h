

#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

typedef struct key_command {
	bool press;
	uint8_t scancode;
	uint8_t ascii;
	uint8_t modifiers;
	bool real;
} key_command_t;

char char_from_code(uint8_t keycode);

void init_keyboard();
key_command_t get_next();

#endif
