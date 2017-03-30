#ifndef SYSTEM_H_
#define SYSTEM_H_
#include <stddef.h>

extern void memcpy(unsigned char *dest, unsigned char *src, size_t count);
extern void memmove(unsigned char *dest, unsigned char *src, size_t count);
extern void memset(unsigned char *src, unsigned char val, size_t count);
extern size_t strlen(char *str);
extern unsigned char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void io_wait(void);

#endif
