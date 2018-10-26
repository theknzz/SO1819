#include "server.h"
#include "client.h"
#include "structs.h"


// Inicialização das env vars

void inicia_vars(editor *t, user *u, server *s)
{
		// editor

		if (getenv("MEDIT_MAXLINES") != NULL)
		{
			t->nlinhas = atoi(getenv("MEDIT_MAXLINES"));
		}

		else
		{
			t->nlinhas = 15;
		}

		if (getenv("MEDIT_MAXCOLUMNS") != NULL)
		{
			t->ncolunas = atoi(getenv("MEDIT_MAXCOLUMNS"));
		}

		else
		{
			t->ncolunas = 45;
		}

		// user

		if (getenv("MEDIT_TIMEOUT") != NULL)
		{
			u->tempo_linha = atoi(getenv("MEDIT_TIMEOUT"));
		}

		else
			u->tempo_linha = 10;

		// server

		if (getenv("MEDIT_MAXUSERS") != NULL)
		{
			s->n_utilizadores_max = atoi(getenv("MEDIT_MAXUSERS"));
		}

		else
			s->n_utilizadores_max = 3;

		// validacao

		if ((s->n_utilizadores_max != t->nlinhas && s->n_utilizadores_max != 3) || t->ncolunas < 0 || t->nlinhas < 0 || u->tempo_linha != 10)
		{
			printf("\nErro na inicialização das variáveis ambiente\n");
	}
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

		void mostra_def(editor t, server s)
		{
			printf("Número de linhas: %d\n", t.nlinhas);
			printf("Número de colunas: %d\n", t.ncolunas);
			printf("Nome da base de dados: %s\n", s.fich_nome);
			printf("Número de named pipes a utilizar: \n"/*s.n_named_pipes*/);
			printf("Nome do named pipe principal: %s\n", s.nome_pipe_p);
		}


// Recebe opcao do servidor

	void getOption(int argc, char **argv, editor t, user u, server s) {

		int c;

		while(1) {

			static struct option long_options[] = {
				{"definicoes", no_argument, 0, 'd'},
				{"carregar", required_argument, 0, 'c'},
				{"guardar", required_argument, 0, 'g'},
				{"liberta", required_argument, 0, 'l'},
				{"estatisticas", no_argument, 0, 'e'},
				{"utilizadores", no_argument, 0, 'u'},
				{"texto", no_argument, 0, 't'},
				{"help", no_argument, 0, 'h'},
				{"sair", no_argument, 0, 's'},
				{0, 0, 0, 0}
			};

			int option_index = 0;

			c = getopt_long (argc,argv,"dc:g:l:eutsh",long_options,&option_index);

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
					mostra_def(t,s);
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
				
				case 'h':

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

	void getUser(int argc, char ** argv,server s)
	{
		int opt;

		while ((opt = getopt(argc,argv, "u:")) != -1)
		{
			switch(opt)
			{
				case 'u':
					verifica_user(optarg,s.fich_nome);
					break;
				
				case '?':
					puts("flag");
					break;

			}
		}
	}