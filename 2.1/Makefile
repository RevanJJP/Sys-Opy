CC = gcc -Wall -std=c11

simple:
	$(CC) -o z2 iosys.c main.c
	make clean
clean:
	rm -f *.o *.a

tests:
	./z2 T text.txt text_copy.txt 4096 4
	./z2 T text.txt text_copy.txt 4096 512
	./z2 T text.txt text_copy.txt 4096 4096
	./z2 T text.txt text_copy.txt 4096 8192