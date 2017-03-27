make
cp grub.cfg iso/boot/grub
cp arch/i386/build/nishikino.bin iso/boot
grub-mkrescue -o nishikino.iso iso/
