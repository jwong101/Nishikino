SOURCES = $(patsubst %.s, %.o, $(wildcard *.s)) $(patsubst %.c, %.o, $(wildcard *.c)) 
BUILD := build
TARGET := nishikino.bin
CC = i686-elf-gcc
AS := i686-elf-as
LDFLAGS := -Tlinker.ld 
CFLAGS := -ffreestanding -O -Wall -Wextra -I./include  

.s.o:
	$(AS) -o $@ -c $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: removeobjs all compile clean

all: checkfolder compile removeobjs

clean: removeobjs
	rm $(BUILD)/$(TARGET)

checkfolder:
	if [ ! -d build ]; then mkdir build; fi

compile: $(SOURCES) linker.ld
	$(CC) $(LDFLAGS) -o $(BUILD)/$(TARGET) -ffreestanding -nostdlib $(SOURCES) -lgcc

removeobjs:
	$(shell find . -name '*.o' -exec rm {} \;)

