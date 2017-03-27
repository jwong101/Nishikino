SOURCES = $(patsubst %.s, %.o, $(wildcard *.s)) $(patsubst %.c, %.o, $(wildcard *.c)) 
BUILD := build
TARGET := nishikino.bin
CC = i686-elf-gcc
LD := i686-elf-ld
AS := i686-elf-as
LDFLAGS := -Tlinker.ld 
#-melf_i386
#CFLAGS := -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -ffreestanding -fno-builtin -I./include/ -g
CFLAGS := -ffreestanding -O -Wall -Wextra -I./include  

.s.o:
	$(AS) -o $@ -c $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: removeobjs all compile

all: compile removeobjs

compile: $(SOURCES) linker.ld
	$(CC) -T linker.ld -o $(BUILD)/$(TARGET) -ffreestanding -nostdlib $(SOURCES) -lgcc

removeobjs:
	$(shell find . -name '*.o' -exec rm {} \;)

