#include "server.h"
#include "client.h"
#include "structs.h"


// Inicialização das env vars

void inicia_vars(editor *t, user *u, server *s)
{
		// Se as variaveis ambiente não existirem são iniciadas com os valores predefinidos no enunciado
		// caso existão o valor delas é passado a inteiro pela funcao 'atoi()' e depois passado à estrutura

		// editor

		if (getenv("MEDIT_MAXLINES") != NULL) 
		{
			t->nlinhas = atoi(getenv("MEDIT_MAXLINES"));
		}

		else
		{
			t->nlinhas = MAXLINES; 
		}

		if (getenv("MEDIT_MAXCOLUMNS") != NULL)
		{
			t->ncolunas = atoi(getenv("MEDIT_MAXCOLUMNS"));
		}

		else
		{
			t->ncolunas = MAXCOLUMNS;
		}

		// user

		if (getenv("MEDIT_TIMEOUT") != NULL)
		{
			u->tempo_linha = atoi(getenv("MEDIT_TIMEOUT"));
		}

		else
			u->tempo_linha = TIME_OUT;

		// server

		if (getenv("MEDIT_MAXUSERS") != NULL)
		{
			s->n_utilizadores_max = atoi(getenv("MEDIT_MAXUSERS"));
		}

		else
			s->n_utilizadores_max = MAXUSERS;

		// validacao

		if ((s->n_utilizadores_max != t->nlinhas && s->n_utilizadores_max != MAXUSERS) || t->ncolunas < 0 || t->nlinhas < 0 || u->tempo_linha != TIME_OUT)
		{
			printf("\nErro na inicialização das variáveis ambiente\n");
		}
} 

// Procura 'nome' recebido por parametro na base de dados recebida por parametro

	int verifica_user( char *nome, char *file) {
		
		char user[8];
		
		FILE  *f = fopen(file,"r");
		if(f == NULL)
		{
			printf("\nErro ao abrir o ficheiro '%s'\n", file); // se a base de dados recebida nao existir ou houver algum problema na sua abertura
			return 0;
		}

			while(fscanf(f," %[^\n]", user) == 1)  // enquanto for possível ler nomes
			{				
				if(strcmp(user, nome) == 0)   // os nomes são comparados
				{
					printf("\nUtilizador '%s' encontrado !\n", nome);  // se forem iguais
					fclose(f);
					return 1;
				}
			}
		
		printf("\nUtilizador '%s' não encontrado...\n", nome); // se nao forem
		fclose(f);
		return 0;
	}

// Mostra defenicoes

		void mostra_def(editor t, server s)
		{
			printf("\n\n\n");
			printf("Número de linhas: %d\n", t.nlinhas);
			printf("Número de colunas: %d\n", t.ncolunas);
			printf("Nome da base de dados: %s\n", s.fich_nome);
			/*printf("Número de named pipes a utilizar: %s\n", s.n_named_pipes);
			printf("Nome do named pipe principal: %s\n", s.nome_pipe_p);*/
		}


// Recebe opcao do servidor

	void getOption(int argc, char **argv, editor t, user u, server s) {

		int c;

		while(1) {
			// comandos 
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

			if(c == -1)   // se o getopt não receber nenhum comando
			{
				break;
			}


			if (optind < argc)
				{
					printf("[ERRO] Comando mal executado, consulte -help.",argv[optind++]);
					putchar('\n');
					exit(0);
				}
			// se receber, compara o comando recebido
			// quer pela long form ou pela short form
			// Exemplo: -d / --definicoes

			switch(c)
			{
				case 0:
					if(long_options[option_index].flag != 0)
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
					printf("'-d'        \tou\tdefinicoes        \tParametros de funcionamento atuais do servidor.\n");
					printf("'-c <fich>' \tou\tcarregar <fich>   \tCarrega dentro dos parametros definidos pelo admin.\n");
					printf("'-g <fich>' \tou\tguardar <fich>    \tGuarda o ficheiro\n");
					printf("'-l <linha>'\tou\tlibertar <linha>  \tDescarta eventuais alteracoes entretato introduzidas.\n");
					printf("'-e'        \tou\testatisticas		\tMostra as estatisticas segundo a segundo.\n");
					printf("'-u'        \tou\tutilizadores      \tMostra todos os utilizadores presentes ordenados por idade de sessao\n");
					printf("'-t'        \tou\ttexto             \tMostar o texto\n");
					printf("'-s'        \tou\tsair              \tTermina a edição em curso emediatamente.\n");
					break;

				case '?':
					printf("\nConsulte -help para listar todos os comandos possiveis.\n");
					break;

				default:
					abort();
			}

			
			exit(0);
		}
	}