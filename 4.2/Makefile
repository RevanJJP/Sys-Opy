CC = gcc -Wall -std=c11

simple:
	$(CC) -o z4 main.c jjp_lib.c
	$(CC) -o child child.c jjp_lib.c
	make clean
clean:
	rm -f *.o *.a