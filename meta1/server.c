#include "server.h"
#include "structs.h"

int main( int argc, char **argv) {

	editor t;
	user u;
	server s;
	int verify = 0;
	
	// Se o utilizador quiser ter acesso ao servidor tem de iniciar sessão

	if(argc == 1)
	{
		printf("Consulte -help para listar todos os comandos possiveis.\n");
	} else {
		
		// problema : até que ponto é que faz sentido um utilizador executar um comando errado e 
		// só depois de iniciar sessão é que recebe o aviso que o comando não funciona?

		printf("\nInicie sessão...\n");
		printf("\nInsira o nome: ");
		scanf(" %7[^\n]", u.nome);
		printf("Insira a base de dados: ");
		scanf(" %9[^\n]", s.fich_nome);
		system("clear");
		verify=verifica_user(u.nome,s.fich_nome);

		if (verify == 1) {
			inicia_vars(&t,&u,&s);
			getOption(argc, argv, t, u, s);
		}
		else
			printf("\nUtilizador '%s' não tem acesso a esse comando.\n",u.nome);
	}
	return 0;
}