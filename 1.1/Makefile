CC = gcc -Wall

simple:
	$(CC) blockarray.c main.c -o zad1

zad1_static:
	$(CC) -c blockarray.c -o blockarray.o
	ar rcs blockarray.a blockarray.o

zad1_shared:
	$(CC) -c -fPIC blockarray.c -o blockarray.o
	$(CC) -shared -fPIC blockarray.o -o blockarray.so

clean:
	rm -f *.o *.so *.a zad1



