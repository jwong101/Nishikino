#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#include <interrupt.h>
#include <tty.h>
#include <system.h>

struct keyboard_packet {
    unsigned char ascii; //0 - 7
    unsigned char key_code; //8 - 15
    unsigned char reserved; //16 - 23
    unsigned char flags; //24 - 31
    unsigned int unicode; //32 - 63
    
} __attribute__ ((packed));

void install_keyboard(void);
void handle_keyboard_interrupt(struct reg_states *regs);


#endif
