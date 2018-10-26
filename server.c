#include "server.h"
#include "structs.h"

int main( int argc, char **argv) {

	editor t;
	user u;
	server s;
	
	// Se o utilizador quiser ter acesso ao servidor tem de iniciar sessão

	if(argc == 1)
	{
		printf("Consulte -help para listar todos os comandos possiveis.\n");
	} else {
		printf("\nInicie sessão...\n");
		printf("\nInsira o nome:");
		scanf(" %7[^\n]", u.nome);
		printf("Insira a base de dados:");
		scanf(" %9[^\n]", s.fich_nome);
		system("clear");
		verifica_user(u.nome,s.fich_nome);
	
		inicia_vars(&t,&u,&s);
		getOption(argc, argv, t, u, s);
	}
	return 0;
}