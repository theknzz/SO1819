all: server client clean 

server: server.o funcoes.o
	gcc server.o funcoes.o -o server

server.o: server.c server.h structs.h
	gcc server.c -c

client: client.o funcoes.o
	gcc client.o funcoes.o -o client -lncurses -g

client.o: client.c client.h structs.h
	gcc client.c -c -g

funcoes.o: funcoes.c
	gcc funcoes.c -c -g

clean:
	rm server.o client.o funcoes.o
