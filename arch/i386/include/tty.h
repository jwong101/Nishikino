#ifndef TTY_H_
#define TTY_H_
#include <stdint.h>

struct cursor {
    int x;
    int y;
};

void initialize_terminal(void);
void clearScreen(void);
void updateHWCursor(void);
void setColor(uint8_t color);
uint16_t calcIndex(uint16_t x, uint16_t y);
void vga_insert(char c, uint8_t color, uint16_t x, uint16_t y);
void vga_putchar(char c);
void vga_write_string(char *str, size_t size);
void kprintf(const char *str, ...);
void kernel_main(void);
void vga_write_int(char c);

#endif
