CC=gcc
CFLAGS=-g -Wall

main: server client


server : 
	gcc -o server server.c

client : 
	gcc -o client client.c

clean :
	rm client
	rm server
