#include "server.h"
#include "client.h"
#include "structs.h"

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

	void getOption(int argc, char *argv) {

		if(strcmp(argv[1],"-s") == 0)
			printf("\n\nSettings:");
		
		if(argc == 2 && strcmp("-l",argv[1]) == 0) {
			printf("Fich: %s", argv[2]);
			printf("\n\nSettings:");
		}

	}

/* server menu */

	void menu() {

		char str[10], fich[10];
		int op;

	do {
		printf("\nMenu: \n");
		printf("settings");
		printf("\nload <filename>");
		printf("\nsave <filename>");
		printf("\nfree <filename>");
		printf("\nstatistics");
		printf("\nusers");
		printf("\ntext\n");

		printf("\nComando : ");
		scanf(" %[^\n]", str);

			if (strcmp(str, "settings")==0)
				op = 1;
			if (strcmp(str, "load")==0)
				op = 2;
			if (strcmp(str, "save")==0)
				op = 3;
			if (strcmp(str, "free")==0)
				op = 4;
			if (strcmp(str, "statistics")==0)
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