
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <vga.h>
#include <gdt.h>
#include <tty.h>
#include <utils.h>
#include <keyboard.h>
#include <timer.h>
#include <interrupt.h>
#include <paging.h>
#include <system.h>


#define VGA_VIDEO_ADDRESS 0xB8000

#define DEFAULT_WIDTH  80
#define DEFAULT_HEIGHT 25
static uint16_t *vbuffp;
static uint8_t fontColor;
static struct cursor cursor;

void initialize_terminal(void) {
    cursor.x = 0;
    cursor.y = 0;
    vbuffp = (uint16_t *)VGA_VIDEO_ADDRESS;
    fontColor = vga_entry_color(GREEN, BLACK);
    clearScreen();
}

void clearScreen(void) {
    for(size_t y = 0; y<DEFAULT_HEIGHT; y++) {
        for(size_t x = 0; x<DEFAULT_WIDTH; x++) {
            const size_t index = y * DEFAULT_WIDTH + x;
            vbuffp[index] = vga_entry(' ', vga_entry_color(WHITE, BLACK));
        }
    }
    cursor.x = 0;
    cursor.y = 0;
}

void updateHWCursor(void) {
    uint16_t index = calcIndex(cursor.x, cursor.y);
    write_port(0x3D4, 0x0F);
    write_port(0x3D5, (uint8_t)(index & 0xFF));

    write_port(0x3D4, 0x0E);
    write_port(0x3D5, (uint8_t)((index >> 8) & 0xFF));
}

void setColor(uint8_t color) {
    fontColor = color;
}

uint16_t calcIndex(uint16_t x, uint16_t y) {
    return y * DEFAULT_WIDTH + x;
}

void vga_insert(char c, uint8_t color, uint16_t x, uint16_t y) {
    if(c == '\n') {
        c = '\0';
        cursor.x = DEFAULT_WIDTH;
    }
    const uint16_t index = calcIndex(x, y);
    *(vbuffp + index) = vga_entry(c, color);
}

void vga_write_int(int print) {
    int store = print;
    int places = 0;

    while(store > 0) {
        places++;
        store /= 10;
    }

    int stack[places];
    store = print;

    int counter = 0;

    while(store > 0) {
        stack[counter++] = store % 10;
        store /= 10;
    }

    int i;
    for(i = LEN(stack) - 1; i >= 0; i--) {
        vga_putchar(stack[i] + 0x30);
    }
}

void vga_putchar(char c) {
    vga_insert(c, fontColor, cursor.x, cursor.y);
    if(++cursor.x >= DEFAULT_WIDTH) {
        cursor.x = 0;
        if(++cursor.y == DEFAULT_HEIGHT) {
            clearScreen();
        }
    }
    updateHWCursor();
}

void vga_write_string(char *str, size_t size) {
    for(size_t i = 0; i<size; i++) {
        vga_putchar(str[i]);
    }
}

void kprintf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    char *s;
    int i;
    const char *iter;
    for(iter = str; *iter != '\0'; ++iter) {
        if(*iter != '%') {
            vga_putchar(*iter);
            continue;
        }
        ++iter;
        switch(*iter) {
        case 'd':
            i = va_arg(args, int);   
            vga_write_int(i);
            break;
        case 's':
            s = va_arg(args, char *);
            vga_write_string(s, strlen(s));
            break;
        case 'c':
            vga_putchar(va_arg(args, char));
            break;
        case '%':
            vga_putchar(*iter);
            break;
        }
    }
}

void kernel_main(void) {
    initialize_terminal();
//    kprintf("Terminal initialized!\n");

   kprintf("Setting up GDT\n");
    setup_gdt();

    kprintf("Setting up Interrupts\n");
    init_idt();

    kprintf("Setting up PIT\n");
    install_timer();

    install_keyboard();


    kprintf("Initializing Kernel Page Directory\n");
    clearScreen();
    initialize_paging();
    kprintf("Initialization of Kernel Page Directory Complete!\n");

    RESUME_INTERRUPTS;
    //Test Page fault
    volatile unsigned int fault = *(unsigned int *)0xA00000000AAAAAAAAAAAAAAAAAAAAAAAAAAAA;

    //unsigned int *ptr = (unsigned int *)0xA000000000000;
    //*ptr = 36;
    //kprintf("%d\n", *ptr); 
    //kprintf("Guess the above didn't happen\n");

    /**
    kprintf("Aishiteru Banzai!\n");
    kprintf("Koko de yokatta");
    kprintf(" watashitachi no ima ga koko ni aru\n");
    kprintf("Aishiteru Banzai!\n");
    kprintf("%s", "Hajimatta bakari ashita mo yoroshiku ne mada gooru janai!");
    kprintf("Yohallo!\n");
    */
    while(1);
}
