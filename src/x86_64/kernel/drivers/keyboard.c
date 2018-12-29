
#include <interrupts.h>
#include <keyboard.h>
#include <serial.h>
#include <IO.h>
#include <util.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define SC_MAX 0x57

uint8_t modifiers = 0;

key_command_t keys[40];
uint8_t insert = 0;
uint8_t pop = 0;

const char *sc_name[] = {
	"ERROR",	 "Esc",		"1", "2", "3", "4",		 "5",
	"6",		 "7",		"8", "9", "0", "-",		 "=",
	"Backspace", "Tab",		"Q", "W", "E", "R",		 "T",
	"Y",		 "U",		"I", "O", "P", "[",		 "]",
	"Enter",	 "Lctrl",   "A", "S", "D", "F",		 "G",
	"H",		 "J",		"K", "L", ";", "'",		 "`",
	"LShift",	"\\",		"Z", "X", "C", "V",		 "B",
	"N",		 "M",		",", ".", "/", "RShift", "Keypad *",
	"LAlt",		 "Spacebar"};

const char sc_ascii[] = {
	'?', 0,   '1', '2',  '3',  '4', '5', '6',  '7', '8', '9', '0',
	'-', '=', 0,   '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
	'o', 'p', '[', ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';',  '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',',  '.',  '/', 0,   '*',  0,   ' ',

	'?', 0,   '!', '@',  '#',  '$', '%', '^',  '&', '*', '(', ')',
	'_', '+', 0,   '\t', 'Q',  'W', 'E', 'R',  'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}',  '\n', 0,   'A', 'S',  'D', 'F', 'G', 'H',
	'J', 'K', 'L', ':',  '"',  '~', '?', '|',  'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<',  '>',  '?', 0,   '*',  0,   ' '};

char char_from_code(uint8_t scancode) {
	if(scancode > SC_MAX) {
		return -1;
	} else if(scancode == ENTER) {
		return '\n';
	} else if(scancode == BACKSPACE) {
		return '\b';
	} else {
		char letter = sc_ascii[scancode];
		if(modifiers & 1) {
			letter = sc_ascii[scancode + 58];
		}
		return letter;
	}
}

bool kbd_handler(registers_t * regs) {
    UNUSED(regs);
	uint8_t incode = inportb(0x60);

	switch(incode & ~0x80) {
		case 0x2A:
		case 0x36:
			modifiers ^= 0b1;
			break;

		default:
			break;
	}
	char res = char_from_code(incode);
	key_command_t cmd;
	cmd.ascii = res;
	cmd.modifiers = 0;
	cmd.press = (incode & 0x80) != 0x80;
	cmd.scancode = incode;
	cmd.real = true;

	keys[insert++ % 40] = cmd;

	key_command_t kct = get_next();
	if(kct.real && kct.press) {
		char text[2] = {kct.ascii, '\0'};
		printf(text);
	} else {
	}

	// printf("0x%x: %s\n", incode, text);
	return true;
}

key_command_t get_next() {
	if(insert == pop) {
		key_command_t kct;
		kct.real = false;
		return kct;
	}
	return keys[(pop++) % 40];
}

bool init_keyboard() {
	outportb(0xF0, 0x01);
	interrupt_register_handler(33, kbd_handler);
	return true;
}