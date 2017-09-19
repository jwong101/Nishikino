#include <keyboard.h>


void install_keyboard(void) {
    install_irq_entry(1, handle_keyboard_interrupt);
}

void handle_keyboard_interrupt(struct reg_states *regs) {
    unsigned char code;
    //0x60 is the data buffer
    code = read_port(0x60);
    if(code & 0x80) {

    } else {
        //print out code
    }
}
