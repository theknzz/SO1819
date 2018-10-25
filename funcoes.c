#include "server.h"
#include "client.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

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

	void verifica_user( char *nome, char *file ) {

		FILE  *f = fopen(file,"r");

		char user[8];

		if(f == NULL)
			printf("\nErro ao abrir o ficheiro");

		while(fscanf( f, " %s\n", user) == 1) {
			if(strcmp(user, nome) == 0){
				printf("\nUtilizador [%s] encontrado !\n", nome);
				fclose(f);
				return;
			}
		}
		printf("\nUtilizador [%s] não encontrado...\n", nome);
		fclose(f);
	}

// Mostra defenicoes

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


// Recebe opcao do servidor

	void getOption(int argc, char **argv) {

		int c;
		int digit_optind = 0;

		while(1) {

			static struct option long_options[] = {
				{"definicoes", no_argument, 0, 'd'},
				{"carregar", required_argument, 0, 'c'},
				{"guardar", required_argument, 0, 'g'},
				{"liberta", required_argument, 0, 'l'},
				{"estatisticas", no_argument, 0, 'e'},
				{"utilizadores", no_argument, 0, 'u'},
				{"texto", no_argument, 0, 't'},
				{"sair", no_argument, 0, 's'},
				{0, 0, 0, 0}
			};

			int option_index = 0;

			c = getopt_long (argc,argv,"dc:g:l:euts",long_options,&option_index);

			if(c == -1)
			{
				break;
			}
			switch(c)
			{
				case 0:
					if(long_options[option_index].flag != 0)
					break;
				printf ("Opcao %s", long_options[option_index].name);
				if (optarg)
					printf(" com o argumento %s", optarg);
				printf("\n");
				break;

				case 'd':
					printf("Opcao -d\n");
					break;

				case 'c':
					printf("Opcao -c com o ficheiro '%s' \n", optarg);
					break;	
				
				case 'g':
					printf("Opcao -g com o ficheiro '%s' \n", optarg);
					break;
				
				case 'l':
					printf("Opcao -l com o numero da linha'%s' \n", optarg);
					break;
				
				case 'e':
					printf("Opcao -e\n");
					break;
				
				case 'u':
					printf("Opcao -u\n");
					break;

				case 't':
					printf("Opcao -t\n");
					break;

				case 's':
					abort();
					break;

				case '?':
					printf("\n  COMANDOS:\n\n");
					printf("\t  '-d'           \tou\tdefinicoes             \t\tParametros de funcionamento atuais do servidor.\n");
					printf("\t  '-c <ficheiro>'\tou\tcarregar <ficheiro>    \t\tCarrega dentro dos parametros definidos pelo admin.\n");
					printf("\t  '-g <ficheiro>'\tou\tguardar <ficheiro>     \t\tGuarda o ficheiro\n");
					printf("\t  '-l <nr_linha>'\tou\tlibertar <nr_linha>    \t\tDescarta eventuais alteracoes entretato introduzidas.\n");
					printf("\t  '-e'           \tou\testatisticas			\t\t(...)\n");
					printf("\t  '-u'           \tou\tutilizadores           \t\tMostra todos os utilizadores presentes ordenados por idade de sessao\n");
					printf("\t  '-t'           \tou\ttexto                  \t\tMostar o texto\n");
					printf("\t  '-s'           \tou\tsair                   \t\tTermina a edição em curso emediatamente.\n");
					break;

				default:
					abort();
			}

			if (optind < argc)
				{
					printf("[ERRO] %s",argv[optind++]);
					putchar('\n');
				}
			exit(0);
		}
	}

/* server menu */

	void menu() {

		char str[20], fich[10];
		int op;

	do {
		printf("\n   Menu:\n");
		printf("\n\tdefinicoes\n");
		printf("\tcarregar <ficheiro>\n");
		printf("\tguardar <ficheiro>\n");
		printf("\tlibertar <nr_linha>\n");
		printf("\testatisticas\n");
		printf("\tutilizadores\n");
		printf("\ttexto\n");
		printf("\tsair\n");

		printf("\nComando : ");
		scanf(" %19[^\n]", str);

			if (strcmp(str, "definicoes")==0)
				op = 1;
			if (strcmp(str, "carregar")==0)
				op = 2;
			if (strcmp(str, "guardar")==0)
				op = 3;
			if (strcmp(str, "libertar")==0)
				op = 4;
			if (strcmp(str, "")==0)
				op = 5;
			if (strcmp(str, "users")==0)
				op = 6;
			if (strcmp(str, "text")==0)
				op = 7;
			if (strcmp(str, "exit")==0)
				op = 8;

				switch(op) {
					case 1: printf("#1"); system("clear");
						printf("%s", str);
						break;
					case 2: printf("#2"); system("clear");
						break;
					case 3: printf("#3"); system("clear");
						break;
					case 4: printf("#4"); system("clear");
						break;
					case 5: printf("#5"); system("clear");
						break;
					case 6: printf("#6"); system("clear");
						break;
					case 7: printf("#7"); system("clear");
						break;
					case 8: system("clear"); return;
					
					default: printf("Opcao invalida ...");
				}

		} while(op > 0 && op < 8);
	}