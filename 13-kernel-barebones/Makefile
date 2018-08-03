all: os-image.bin

run: all
	qemu-system-i386 -fda os-image.bin

clean:
	rm *.bin *.o

os-image.bin : bootsect.bin kernel.bin
	cat bootsect.bin kernel.bin > os-image.bin

kernel.bin : kernel_entry.o kernel.o
	i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary

kernel.o : kernel.c
	i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o

kernel_entry.o : kernel_entry.asm
	nasm kernel_entry.asm -f elf -o kernel_entry.o

bootsect.bin : bootsect.asm
	nasm bootsect.asm -f bin -o bootsect.bin