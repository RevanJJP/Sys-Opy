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

staticO0:
	$(CC) -c $(LIB).c -o $(LIB).o -O0
	ar rcs lib$(LIB).a $(LIB).o
	$(CC) -o main_static main.c lib$(LIB).a -I . -O0
dynamicO0:
	$(CC) -fPIC -c $(LIB).c -o $(LIB).o -O0
	$(CC) -Wl,-rpath=. -shared -fPIC -o lib$(LIB).so $(LIB).o -O0
	$(CC) -g -L. -o $(MAIN)_dynamic $(MAIN).c -ldl -D DLL -O0

sharedO0:
	$(CC) -c -fPIC $(LIB).c -o $(LIB).o -O0
	$(CC) -shared -fPIC $(LIB).o -o lib$(LIB).so -O0
	$(CC) -Wl,-rpath=. -L. -o $(MAIN)_shared $(MAIN).c -l $(LIB) -O0

staticO1:
	$(CC) -c $(LIB).c -o $(LIB).o -O1
	ar rcs lib$(LIB).a $(LIB).o
	$(CC) -o main_static main.c lib$(LIB).a -I . -O1
dynamicO1:
	$(CC) -fPIC -c $(LIB).c -o $(LIB).o -O1
	$(CC) -Wl,-rpath=. -shared -fPIC -o lib$(LIB).so $(LIB).o -O1
	$(CC) -g -L. -o $(MAIN)_dynamic $(MAIN).c -ldl -D DLL -O1
sharedO1:
	$(CC) -c -fPIC $(LIB).c -o $(LIB).o -O1
	$(CC) -shared -fPIC $(LIB).o -o lib$(LIB).so -O1
	$(CC) -Wl,-rpath=. -L. -o $(MAIN)_shared $(MAIN).c -l $(LIB) -O1

staticO2:
	$(CC) -c $(LIB).c -o $(LIB).o -O2
	ar rcs lib$(LIB).a $(LIB).o
	$(CC) -o main_static main.c lib$(LIB).a -I . -O2
dynamicO2:
	$(CC) -fPIC -c $(LIB).c -o $(LIB).o -O2
	$(CC) -Wl,-rpath=. -shared -fPIC -o lib$(LIB).so $(LIB).o -O2
	$(CC) -g -L. -o $(MAIN)_dynamic $(MAIN).c -ldl -D DLL -O2
sharedO2:
	$(CC) -c -fPIC $(LIB).c -o $(LIB).o -O2
	$(CC) -shared -fPIC $(LIB).o -o lib$(LIB).so -O2
	$(CC) -Wl,-rpath=. -L. -o $(MAIN)_shared $(MAIN).c -l $(LIB) -O2

staticO3:
	$(CC) -c $(LIB).c -o $(LIB).o -O3 
	ar rcs lib$(LIB).a $(LIB).o
	$(CC) -o main_static main.c lib$(LIB).a -I . -O3
dynamicO3:
	$(CC) -fPIC -c $(LIB).c -o $(LIB).o -O3
	$(CC) -Wl,-rpath=. -shared -fPIC -o lib$(LIB).so $(LIB).o -O3
	$(CC) -g -L. -o $(MAIN)_dynamic $(MAIN).c -ldl -D DLL -O3
sharedO3:
	$(CC) -c -fPIC $(LIB).c -o $(LIB).o -O3
	$(CC) -shared -fPIC $(LIB).o -o lib$(LIB).so -O3
	$(CC) -Wl,-rpath=. -L. -o $(MAIN)_shared $(MAIN).c -l $(LIB) -O3

staticOs:
	$(CC) -c $(LIB).c -o $(LIB).o -Os
	ar rcs lib$(LIB).a $(LIB).o # statyczna library.a
	$(CC) -o main_static main.c lib$(LIB).a -I . -Os
dynamicOs:
	$(CC) -fPIC -c $(LIB).c -o $(LIB).o -Os
	$(CC) -Wl,-rpath=. -shared -fPIC -o lib$(LIB).so $(LIB).o -Os
	$(CC) -g -L. -o $(MAIN)_dynamic $(MAIN).c -ldl -D DLL -Os
sharedOs:
	$(CC) -c -fPIC $(LIB).c -o $(LIB).o -Os
	$(CC) -shared -fPIC $(LIB).o -o lib$(LIB).so -Os
	$(CC) -Wl,-rpath=. -L. -o $(MAIN)_shared $(MAIN).c -l $(LIB) -Os

