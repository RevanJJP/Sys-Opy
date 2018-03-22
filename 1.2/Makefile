CC = gcc -Wall -std=c11
LIB = blockarray
MAIN = main

simple:
	$(CC) blockarray.c main.c -o zad2

zad2_static:
	$(CC) -c blockarray.c -o blockarray.o
	ar rcs blockarray.a blockarray.o

zad2_shared:
	$(CC) -c -fPIC blockarray.c -o blockarray.o
	$(CC) -shared -fPIC blockarray.o -o blockarray.so

clean:
	rm -f *.o *.so *.a zad2



