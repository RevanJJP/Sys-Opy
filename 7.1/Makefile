CC = gcc -Wall -std=c11

all:
	$(CC) -o client Client.c jjp_lib.c
	$(CC) -o barber Barber.c jjp_lib.c
	make clean
clean:
	rm -f *.o *.a