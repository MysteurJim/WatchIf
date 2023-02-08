all : clean client server


client: client.o
	gcc -o client client.o

client.o:
	gcc -c client.c

server: server.o
	gcc -o server server.o

server.o:
	gcc -c server.c

clean:
	rm -f *.o server client

