all: client server

client: client.c time.c time.h
	gcc -g -Wall -o client client.c time.c client_util.c

server: server.c
	gcc -g -Wall -o server server.c socket_node.c server_util.c web_mode.c

clean: 
	rm -f *.o
	rm -f *~
	rm -f core
	rm -f client
	rm -f server
