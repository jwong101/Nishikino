#include <timer.h>
#include <tty.h>
#include <interrupt.h>
#include <system.h>

static unsigned long total_ticks;

void timer_wait(unsigned int ticks) {
    unsigned long target = ticks + total_ticks;
    while(ticks < target);
}

void handle_timer_interrupt(struct reg_states *regs) {
    ++total_ticks;
    if(total_ticks % 18 == 0) {
        kprintf("Ohayo");
    }
}

void install_timer(void) {
    set_timer_clock(10);
    install_irq_entry(0, handle_timer_interrupt);
}

/**
*[counter 7 - 6] [rw 1=lsb 2=msb 3=both 5 - 4] [mode 3 - 1] [bcd 0]
* modes
* 0 = interrupt on terminal count
* 1 = hardware retriggerable one shot
* 2 = rate generator
* 3 = square wave mode
* 4 = software strobe
* 5 = hardware strobe
*/
unsigned char calculate_command(unsigned char counter, unsigned char rw, unsigned char mode, unsigned char bcd) {
    unsigned char command = 0;
    command |= (counter << 6) & 0xC0;
    command |= (rw << 4) & 0x30;
    command |= (mode << 1) & 0x0E;
    command |= bcd & 0x01;
    return command;
}

/**
 * Sets the timer frequency
 * The timer determines the frequency by dividing the INITIAL_FREQUENCY by the value entered into PIC_CHANNEL_0
 */
void set_timer_clock(unsigned short hz) {
    unsigned short divisor = INITIAL_FREQUENCY / hz;
    unsigned char command = calculate_command(0, 8, 3, 0);
    write_port(PIC_COMMAND, command);
    //send lsb then msb
    write_port(PIC_CHANNEL_0, divisor & 0xFF);
    write_port(PIC_CHANNEL_0, divisor >> 8);
}
