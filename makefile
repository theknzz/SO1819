all: server clean

servidor: servidor.o funcoes.o
	gcc servidor.o funcoes.o -o servidor

servidor.o: servidor.c
	gcc servidor.c -c

funcoes.o: funcoes.c
	gcc funcoes.c -c

clean: servidor
	rm *.o servidor
