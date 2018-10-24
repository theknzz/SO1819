#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "medit-defaults.h"
#include "cliente-defaults.h"
#include "server-defaults.h"
#include "structs.h"


int main( int argc, char *argv) {

	inicia_vars();

	//printf("Comando: ");
	//fgets(str, 100, stdin);
	//str[strlen(str)-1] = '\0';

	//menu();

	printf("Insira um nome: ");
	scanf(" %7[^\n]", user.nome);

	printf("Insira um fich: ");
	scanf(" %9[^\n]", fich.nome);

	verifica_user(user.nome,fich.nome);
	return 0;
}
