#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "medit-defaults.h"
#include "server-defaults.h"
#include "structs.h"


int main( int argc, char *argv) {

	inicia_vars();
	user ut;
	fich fx;

	//printf("Comando: ");
	//fgets(str, 100, stdin);
	//str[strlen(str)-1] = '\0';

	//menu();

	printf("Insira um nome: ");
	scanf(" %7[^\n]", ut.nome);

	printf("Insira um fich: ");
	scanf(" %9[^\n]", fx.nome);

	verifica_user(ut.nome,fx.nome);

	return 0;
}
