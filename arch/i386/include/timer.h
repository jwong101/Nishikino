#ifndef TIMER_H_
#define TIMER_H_
#include <interrupt.h>

#define PIC_CHANNEL_0 0x40
#define PIC_CHANNEL_1 0x41
#define PIC_CHANNEL_2 0x42
#define PIC_COMMAND 0x43

#define INITIAL_FREQUENCY 1193180
void handle_timer_interrupt(struct reg_states *regs);
void timer_wait(unsigned int ticks);
void set_timer_clock(unsigned short hz);
void install_timer(void);

#endif
