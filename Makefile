
ALL:
	gcc client.c -o client
	gcc server.c link.c -o server -lpthread -lmysqlclient
clean:
	rm client server
