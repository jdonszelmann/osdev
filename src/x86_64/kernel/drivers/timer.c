
#include <timer.h>
#include <interrupts.h>

uint64_t tick = 0;

bool timer_callback()
{
    tick++;
    printf("%i\n", tick);

    return true;
}

void init_timer(uint32_t freq)
{
    /* Install the function we just wrote */

    interrupt_register_handler(32, timer_callback);
    printf("%i\n", interrupt_check_handler(32));

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / freq;
    printf("Running at %i Hz\n", divisor);
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) % 0xFF);

    /* Send the command */
    outportb(0x43, 0x36); /* Command port */
    outportb(0x40, low);
    outportb(0x40, high);
}