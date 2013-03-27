all: server client

server: server.o SharedFileLinkedList.o ShareClient.o ShareClientList.o

client: client.o Message.o MessageList.o

server.o: server.c
	gcc -g -c server.c

SharedFileLinkedList.o: SharedFileLinkedList.c
	gcc -g -c SharedFileLinkedList.c

ShareClient.o: ShareClient.c
	gcc -g -c ShareClient.c

ShareClientList.o: ShareClientList.c
	gcc -g -c ShareClientList.c

client.o: client.c
	gcc -g -c client.c

Message.o: Message.c
	gcc -g -c Message.c

MessageList.o: MessageList.c
	gcc -g -c MessageList.c

clean:
	rm -rf *.o server client
