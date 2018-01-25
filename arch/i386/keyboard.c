#include <keyboard.h>
unsigned char scancodes[127] =
{
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 0 - 9 */
	'9', '0', '-', '=', '\b',				/* Backspace */
	'\t',							/* Tab */
	'q', 'w', 'e', 'r',					/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
	0,							/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	
    /* 39 */
	'\'', '`',   0,					/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',   0,				/* Right shift */
	'*',
	0,							/* Alt */
	' ',							/* Space bar */
	0,							/* Caps lock */
    /**59 - 68 F1 - F10 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,							/* 69 - Num lock*/
	0,							/* Scroll Lock */
	0,							/* Home key */
	0,							/* Up Arrow */
	0,							/* Page Up */
  '-',
	0,							/* Left Arrow */
	0,
	0,							/* Right Arrow */
  '+',
	0,							/* 79 - End key*/
	0,							/* Down Arrow */
	0,							/* Page Down */
	0,							/* Insert Key */
	0,							/* Delete Key */
	0,   0,   0,
	0,							/* F11 Key */
	0,							/* F12 Key */
	0,							/* Undefined */
};	

void enable_a20_gate(void) {
    int portVal = read_port(0x92);
    portVal | 2;
    write_port(0x92, portVal);
}

void install_keyboard(void) {
//    enable_a20_gate();
    install_irq_entry(1, handle_keyboard_interrupt);
}

void handle_keyboard_interrupt(struct reg_states *regs) {
    unsigned char code;
    //0x60 is the data buffer
    code = read_port(0x60);
    if(code & 0x80) {
        
    } else if(code >= 0 && code < 127) {
        //print out code
        vga_putchar(scancodes[code]);
    }
}
