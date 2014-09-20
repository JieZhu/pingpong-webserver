all: client server

client: client.c time.c time.h
	gcc -g -Wall -o client client.c time.c

server: server.c
	gcc -g -Wall -o server server.c

clean: 
	rm -f *.o
	rm -f *~
	rm -f core
	rm -f client
	rm -f server
