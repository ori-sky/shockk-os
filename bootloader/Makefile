ASM=nasm

bootloader: bootsector
	cat bootsector.bin > bootloader.bin

bootsector: bootsector.asm
	$(ASM) -f bin $^ -o bootsector.bin

.PHONY: clean
clean:
	rm -fv bootsector.bin
