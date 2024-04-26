CC = gcc
CFLAGS = -Wall -pthread

all: http_client http_server

http_client: http_client.o
	$(CC) $(CFLAGS) http_client.o -o http_client

http_server: http_server.o
	$(CC) $(CFLAGS) http_server.o -o http_server

http_client.o: http_client.c
	$(CC) $(CFLAGS) -c http_client.c

http_server.o: http_server.c
	$(CC) $(CFLAGS) -c http_server.c

clean:
	rm -f *.o http_client http_server
