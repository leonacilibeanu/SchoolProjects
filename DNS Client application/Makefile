all: my_dns_client

my_dns_client: my_dns_client.o

my_dns_client.o: my_dns_client.c
	gcc -fpack-struct -g -c my_dns_client.c

clean:
	rm -rf *.o my_dns_client
