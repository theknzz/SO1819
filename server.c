#include "server.h"
#include "structs.h"

int main( int argc, char **argv) {
	editor t;
	user u;
	server s;

	s.fich_nome[10] = "medit.db";
	

	char fich[10];
	inicia_vars(&t,&u,&s);
	getOption(argc, argv, t, u, s);
	

	printf("Insira um nome: ");
	scanf(" %7[^\n]", u.nome);

	verifica_user(u.nome,s.fich_nome);

	return 0;
}