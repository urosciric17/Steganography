CC=gcc
FLAGS=-Wall
steganography:steganography.c
	@$(CC) $(FLAGS) steganography.c -o steganography -lfreeimage
	@chmod +x steganography
	@$(CC) $(FLAGS) steganography.c -o steganography -lfreeimage
	@chmod +x steganography
clean:
	@rm modified_hw
	@rm modified_test.png
run1:
	@./steganography
run2:
	@./steganography2
