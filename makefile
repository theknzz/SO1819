server: server.o funcoes.o
	gcc server.o funcoes.o -o server

server.o: server.c server.h structs.h
	gcc server.c -c

funcoes.o: funcoes.c
	gcc funcoes.c -c
