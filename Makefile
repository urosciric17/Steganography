CC=gcc
FLAGS=-Wall
steganography:steganography.c
	@$(CC) $(FLAGS) steganography.c -o steganography -lfreeimage
	@chmod +x steganography
steganography2:steganography2.c
	@$(CC) $(FLAGS) steganography2.c -o steganography2 -lfreeimage
	@chmod +x steganography2
clean:
	@rm modified_hw
	@rm modified_test.png
run1:
	@./steganography
run2:
	@./steganography2
