#include <system.h>

void memcpy(unsigned char *dest, unsigned char *src, size_t count) {
    size_t i;
    for(i = 0; i<count; i++) {
        dest[i] = src[i];
    }
}

void memmove(unsigned char *dest, unsigned char *src, size_t count) {
    if(src >= dest) {
        while(count--) {
            *dest++ = *src++;
        }
    } else {
        while(count--) {
            dest[count] = src[count];
        }
    }
}

void memset(unsigned char *src, unsigned char val, size_t count) {
    size_t i;
    for(i = 0; i<count; i++) {
        src[i] = val;
    }
}

size_t strlen(char *str) {
    size_t len = 0;
    while(str[len] != '\0') {
        ++len;
    }
    return len;
}

unsigned char read_port(unsigned short port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void write_port(unsigned short port, unsigned char data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}
