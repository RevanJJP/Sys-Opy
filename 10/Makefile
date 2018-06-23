CC = gcc -Wall -std=c11
LIBS = -pthread

all: clean server client

server:
	$(CC) -o server server.c jjp_lib.c $(LIBS)

client:
	$(CC) -o client client.c jjp_lib.c $(LIBS)

clean:
	rm -f *.o *.a
