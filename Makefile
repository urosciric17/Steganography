CC=gcc
FLAGS=-Wall
steganography:steganography.c
	@$(CC) $(FLAGS) steganography.c -o steganography -lfreeimage
	@chmod +x steganography
clean:
	@rm steganography
	@rm modified_test.png
run:
	@./steganography
