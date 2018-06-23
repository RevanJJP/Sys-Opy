CC = gcc -Wall -std=c11

simple:
	$(CC) -o client client.c jjp_lib.c
	$(CC) -o server server.c jjp_lib.c
	make clean
clean:
	rm -f *.o *.a