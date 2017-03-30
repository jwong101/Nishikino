#include <keyboard.h>

/**
*unsigned char scancodes[101] = {
*    0, 0 ** esc *, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
*    '-', 
*};
*/

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
