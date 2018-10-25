#include "server.h"
#include "client.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

// Inicialização das env vars

	void inicia_vars() {

		if (setenv("MEDIT_MAXLINES","15",1) == 0 && setenv("MEDIT_MAXCOLUMNS","45",1) == 0)
			return;

		printf("\nErro na inicialização das variaveis ambiente");
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

// Recebe opcão do server [ERRO]

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