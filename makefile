all: server client clean 


server: server.o funcoes.o
	gcc server.o funcoes.o -o server -lpthread -g

server.o: server.c server.h structs.h
	gcc server.c -c

client: client.o funcoes.o ncurses.o
	gcc client.o funcoes.o ncurses.o -o client -lncurses -lpthread -g

client.o: client.c client.h structs.h
	gcc client.c -c

funcoes.o: funcoes.c
	gcc funcoes.c -c -lpthread

ncurses.o: ncurses.c
	gcc ncurses.c -c

clean:
	rm server.o client.o funcoes.o ncurses.o sss sss0 sss1 sss2
