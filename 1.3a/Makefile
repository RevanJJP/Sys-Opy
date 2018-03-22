CC = gcc -Wall -std=c11
LIB = blockarray
MAIN = main

simple:
	$(CC) $(LIB).c $(MAIN).c -o zad2

zad2_static:
	$(CC) -c $(LIB).c -o $(LIB).o
	ar rcs $(LIB).a $(LIB).o

zad2_shared:
	$(CC) -c -fPIC $(LIB).c -o $(LIB).o
	$(CC) -shared -fPIC $(LIB).o -o lib$(LIB).so
clean:
	rm -f *.o *.so *.a zad2 main_static main_dynamic main_shared

zad3a_static:
	make zad2_static
	$(CC) -o main_static main.c blockarray.a -I .

zad3a_dynamic:
	$(CC) -fPIC -c $(LIB).c -o $(LIB).o
	$(CC) -Wl,-rpath=. -shared -fPIC -o lib$(LIB).so $(LIB).o
	$(CC) -g -L. -o $(MAIN)_dynamic $(MAIN).c -ldl -D DLL

zad3a_shared:
	make zad2_shared
	$(CC) -Wl,-rpath=. -L. -o $(MAIN)_shared $(MAIN).c -l $(LIB)

static_tests:
	./main_static C S 1000 100
	./main_static S S 1000
	./main_static R S 1000
	./main_static A S 1000
	./main_static N S 1000
	./main_static C D 1000 100
	./main_static S D 1000
	./main_static R D 1000
	./main_static A D 1000
	./main_static N D 1000

dynamic_tests:
	./main_dynamic C S 1000 100
	./main_dynamic S S 1000
	./main_dynamic R S 1000
	./main_dynamic A S 1000
	./main_dynamic N S 1000
	./main_dynamic C D 1000
	./main_dynamic S D 1000
	./main_dynamic R D 1000
	./main_dynamic A D 1000
	./main_dynamic N D 1000

shared_tests:
	./main_shared C S 1000 100
	./main_shared S S 1000
	./main_shared R S 1000
	./main_shared A S 1000
	./main_shared N S 1000
	./main_shared C D 1000 100
	./main_shared S D 1000
	./main_shared R D 1000
	./main_shared A D 1000
	./main_shared N D 1000
