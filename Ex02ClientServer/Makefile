CC=gcc
CFLAGS=-Wall

all: server client

# Usage: ./server <port> <ssh_executable> <ssh_destination> <client_executable>
#   Just an example below. Modify Makefile accordingly.
run:
	@echo "Change Makefile with proper hostname and path to client executable"
	./server 8111 /usr/bin/ssh localhost /home/ubuntu/Host/Ex02ClientServer/client


server: server.c
	$(CC) $(CFLAGS) server.c -o server

# Usage: ./client <hostname> <port>
client: client.c
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f server client

runserveronly:
	./server 8000 /usr/bin/ssh localhost /anypath

runclient:
	./client localhost 8000
