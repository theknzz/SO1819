#include "server.h"
#include "client.h"
#include "structs.h"

// Inicialização das env vars

void inicia_vars()
{
	editor t;
	user u;
	server s;

	if ((t.nlinhas = getenv("MEDIT_MAXLINES")) == NULL)
	{
		t.nlinhas = 15;
	}
	else if ((t.ncolunas = getenv("MEDIT_MAXLINES")) == NULL)
	{
		t.ncolunas = 45;
	}
	else if ((u.tempo_linha = getenv("MEDIT_TIMEOUT")) == NULL)
	{
		u.tempo_linha = 10;
	}
	else if ((s.n_utilizadores = getenv("MEDIT_MAXUSERS")) == NULL)
	{
		s.n_utilizadores = 3;
	}
	else if ((s.n_utilizadores != t.nlinhas &&  s.n_utilizadores != 3) || t.ncolunas < 0 || t.nlinhas < 0 || u.tempo_linha != 10 || s.n_utilizadores != t.nlinhas)
	{
		printf("\nErro na inicialização das variáveis ambiente\n");
		exit(1);
	}
	exit(0);
}

// compara cada nome do ficheiro recebido com "nome"

void verifica_user(char *nome, char *file)
{
	FILE *f = fopen(file, "r");

	char user[8];

	if (f == NULL)
		printf("\nErro ao abrir o ficheiro");

	while (fscanf(f, " %s\n", user) == 1)
	{
		if (strcmp(user, nome) == 0)
		{
			printf("\nUtilizador [%s] encontrado !\n", nome);
			fclose(f);
			return;
		}
	}
	printf("\nUtilizador [%s] não encontrado...\n", nome);
	fclose(f);
}

// Recebe opcão do server [ERRO]

void getOption(int argc, char *argv)
{
	if (strcmp(argv[1], "-s") == 0)
		printf("\n\nSettings:");

	if (argc == 2 && strcmp("-l", argv[1]) == 0)
	{
		printf("Fich: %s", argv[2]);
		printf("\n\nSettings:");
	}
}

void mostra_def()
{
	editor t;
	server s;

	printf("Número de linhas: %d\n", t.nlinhas);
	printf("Número de colunas: %d\n", t.ncolunas);
	printf("Nome da base de dados: %s\n", s.fich_nome);
	printf("Número de named pipes a utilizar: %d\n", s.n_named_pipes);
	printf("Nome do named pipe principal: %s\n", s.nome_pipe_p);
}

/* server menu */

void menu()
{
	char str[10], fich[10];
	int op;

	do
	{
		printf("\nMenu:");
		printf("\nsettings");
		printf("\nload <filename>");
		printf("\nsave <filename>");
		printf("\nfree <filename>");
		printf("\nstatistics");
		printf("\nusers");
		printf("\ntext\n");

		printf("\nComando : ");
		scanf(" %[^\n]", str);

		if (strcmp(str, "settings") == 0)
			op = 1;
		if (strcmp(str, "load") == 0)
			op = 2;
		if (strcmp(str, "save") == 0)
			op = 3;
		if (strcmp(str, "free") == 0)
			op = 4;
		if (strcmp(str, "statistics") == 0)
			op = 5;
		if (strcmp(str, "users") == 0)
			op = 6;
		if (strcmp(str, "text") == 0)
			op = 7;
		if (strcmp(str, "exit") == 0)
			op = 8;

		switch (op)
		{
		case 1:
			printf("Os seus settings atuais são os seguintes:\n\n");
			mostra_def();
			system("clear");
			break;
		case 2:
			printf("#2");
			system("clear");
			break;
		case 3:
			printf("#3");
			system("clear");
			break;
		case 4:
			printf("#4");
			system("clear");
			break;
		case 5:
			printf("#5");
			system("clear");
			break;
		case 6:
			printf("#6");
			system("clear");
			break;
		case 7:
			printf("#7");
			system("clear");
			break;
		case 8:
			system("clear");
			return;
		}

	} while (op > 0 && op < 8);
}