
#include <keyboard.h>
#include <interrupts.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C

const int8_t sc_max = 0x57;

int8_t shift_in = 0;

const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = {'?', 0, '1', '2', '3', '4', '5', '6', '7', '8',
                         '9', '0', '-', '=', 0, '\t', 'q', 'w', 'e', 'r',
                         't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
                         'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
                         '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 'b', 'n',
                         'm', ',', '.', '/', 0, '*', 0, ' ',

                         '?', 0, '!', '@', '#', '$', '%', '^', '&', '*',
                         '(', ')', '_', '+', 0, '\t', 'Q', 'W', 'E', 'R',
                         'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
                         'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
                         '"', '~', '?', '|', 'Z', 'X', 'C', 'V', 'B', 'N',
                         'M', '<', '>', '?', 0, '*', 0, ' '};

char char_from_code(int8_t scancode)
{
    // if (strcmp((char*)sc_name[scancode], "LShift") == 0 ||
    //     strcmp((char*)sc_name[scancode], "RShift") == 0)
    // {
    //     shift_in = 58;
    //     return -1;
    // }
    // else if ((scancode > 0x80) &&
    //          (strcmp((char*)sc_name[scancode - 0x80], "LShift") == 0 ||
    //           strcmp((char*)sc_name[scancode - 0x80], "RShift") == 0))
    // {
    //     shift_in = 0;
    //     return -1;
    // }
    // else
    if (scancode > sc_max)
        return -1;

    else if (scancode == ENTER)
    {

        return '\n';
    }
    else
    {
        char letter = sc_ascii[scancode];
        return letter;
    }
}

bool kbd_handler()
{
    int8_t incode = inportb(0x60);
    char res = char_from_code(incode);
    if (res < 0)
        return true;
    char text[2] = {res, '\0'};
    printf("%x: %s\n",incode,text);
    return true;
}

void init_keyboard()
{
    interrupt_register_handler(33, kbd_handler);
}