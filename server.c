#include "server.h"
#include "structs.h"

int main( int argc, char *argv) {

	inicia_vars();
	user ut;
	fich fx;

	/* if (argc == 1)
			menu();
	else */
		getOption(argc,argv);
	

	/* printf("Insira um nome: ");
	scanf(" %7[^\n]", ut.nome);

	printf("Insira um fich: ");
	scanf(" %9[^\n]", fx.nome);

	verifica_user(ut.nome,fx.nome); */

	return 0;
}
