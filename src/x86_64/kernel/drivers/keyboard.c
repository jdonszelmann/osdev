
#include <IO.h>
#include <interrupts.h>
#include <keyboard.h>
#include <serial.h>
#include <util.h>
#include <kheap.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SPACE 0x39

#define SC_MAX 0x57

uint8_t modifiers = 0;

key_command_t keys[40];
uint8_t insert = 0;
uint8_t pop = 0;
uint8_t codes = 0;

const char *sc_name[] = {
	"ERROR",	 "Esc",		 "1",		"2", "3", "4",		"5",
	"6",		 "7",		 "8",		"9", "0", "-",		"=",
	"Backspace", "Tab",		 "Q",		"W", "E", "R",		"T",
	"Y",		 "U",		 "I",		"O", "P", "[",		"]",
	"Enter",	 "Lctrl",	"A",		"S", "D", "F",		"G",
	"H",		 "J",		 "K",		"L", ";", "'",		"`",
	"LShift",	"\\",		 "Z",		"X", "C", "V",		"B",
	"N",		 "M",		 ",",		".", "/", "RShift", "Keypad *",
	"LAlt",		 "Spacebar", "CapsLock"};

const char sc_ascii[] = {'?',  0,   '1', '2',  '3', '4',  '5', '6', '7',  '8',
						 '9',  '0', '-', '=',  0,   '\t', 'q', 'w', 'e',  'r',
						 't',  'y', 'u', 'i',  'o', 'p',  '[', ']', '\n', 0,
						 'a',  's', 'd', 'f',  'g', 'h',  'j', 'k', 'l',  ';',
						 '\'', '`', '?', '\\', 'z', 'x',  'c', 'v', 'b',  'n',
						 'm',  ',', '.', '/',  0,   '*',  0,   ' '};

const char sc_ascii_caps[] = {
	'?',  0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',  0,
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
	'A',  'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '?', '|',  'Z',
	'X',  'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' '};

uint32_t strlenq(const char *str) {
	for(uint32_t i = 0;; i++) {
		if(str[i] == '\0') {
			return i;
		}
	}
	return 0;
}

char char_from_code(uint8_t scancode) {
#if DEBUG
	LOG("Code: %x", scancode);
#endif
	if(scancode > SC_MAX) {
		return 0;
	} else if(scancode == ENTER) {
		return '\n';
	} else if(scancode == BACKSPACE) {
		return '\b';
	} else if(scancode == SPACE) {
		return ' ';
	} else if(strlenq(sc_name[scancode]) > 1) {
		return 0;
	} else {
		char letter = sc_ascii[scancode];
		if(modifiers & 0b100) {
			letter = sc_ascii_caps[scancode];
		}
		return letter;
	}
}

bool kbd_handler(registers_t *regs) {
	UNUSED(regs);
	uint8_t incode = inportb(0x60);

	// Temporary Switches
	switch(incode & ~0x80) {
		case 0x2A:
		case 0x36:
			modifiers ^= 0b100;
			break;

		default:
			break;
	}

	// Lock Switches
	switch(incode) {
		case 0x3A:
			modifiers ^= 0b100;
			codes = modifiers;
			outportb(0xED, codes);
			break;
		case 0x45:
			modifiers ^= 0b10;
			codes = modifiers;
			outportb(0xED, codes);
			break;
		case 0x46:
			modifiers ^= 0b1;
			codes = modifiers;
			outportb(0xED, codes);
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
	char* str = print_key_command(kct);
	LOG(str);
	if(kct.real && kct.press && kct.ascii != 0) {
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

char *print_key_command(key_command_t command) {
	int32_t len = sprintf(
		NULL, "KeyCommand(scancode=%x, ascii=%c, mods=%b, press=%i, real=%i)\n",
		command.scancode, command.ascii, command.modifiers, command.press,
		command.real);
	char* str = kmalloc(len);
	
	sprintf(
		str, "KeyCommand(scancode=%x, ascii=%c, mods=%b, press=%i, real=%i)\n",
		command.scancode, command.ascii, command.modifiers, command.press,
		command.real);
	return str;
}

bool init_keyboard() {
	outportb(0xF0, 0x01);
	interrupt_register_handler(33, kbd_handler);
	return true;
}