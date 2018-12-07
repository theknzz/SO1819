#include "server.h"
#include "client.h"
#include "structs.h"
// ------------------------------------------------------------------------------------------------------
//                                  db    db  .d8b.  d8888b. .d8888.
//                                  88    88 d8' `8b 88  `8D 88'  YP
//                                  Y8    8P 88ooo88 88oobY' `8bo.
//                                  `8b  d8' 88~~~88 88`8b     `Y8b.
//                                   `8bd8'  88   88 88 `88. db   8D
//                                     YP    YP   YP 88   YD `8888Y'
// ------------------------------------------------------------------------------------------------------

// (...)

// ------------------------------------------------------------------------------------------------------
//                            d888b  db       .d88b.  d8888b.  .d8b.  db
//                           88' Y8b 88      .8P  Y8. 88  `8D d8' `8b 88
//                           88      88      88    88 88oooY' 88ooo88 88
//                           88  ooo 88      88    88 88~~~b. 88~~~88 88
//                           88. ~8~ 88booo. `8b  d8' 88   8D 88   88 88booo.
//                            Y888P  Y88888P  `Y88P'  Y8888P' YP   YP Y88888P
// ------------------------------------------------------------------------------------------------------

// Inicialização das env vars
void inicia_vars(editor *t, user *u, server *s)
{
	// Se as variaveis ambiente não existirem são iniciadas com os valores predefinidos no enunciado
	// caso existão o valor delas é passado a inteiro pela funcao 'atoi()' e depois passado à estrutura

	// base de dados por omissão
	strcpy(s->fich_nome, "medit.db");

	s->n_named_pipes = MAXUSERS;
	strcpy(s->nome_pipe_p, SERVER_FIFO_P);

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
		exit(EXIT_FAILURE);
	}
}

// ------------------------------------------------------------------------------------------------------
//                       .o88b. db      d888888b d88888b d8b   db d888888b d88888b
//                      d8P  Y8 88        `88'   88'     888o  88 `~~88~~' 88'
//                      8P      88         88    88ooooo 88V8o 88    88    88ooooo
//                      8b      88         88    88~~~~~ 88 V8o88    88    88~~~~~
//                      Y8b  d8 88booo.   .88.   88.     88  V888    88    88.
//                       `Y88P' Y88888P Y888888P Y88888P VP   V8P    YP    Y88888P
// ------------------------------------------------------------------------------------------------------

// Recebe opcao do utilizador
void getOption_cli(int argc, char **argv, user *u)
{
	int c;

	while (1)
	{
		int option_index = 0;
		c = getopt(argc, argv, "u:");

		if (c == -1)
			break;

		if (optind < argc)
		{
			fprintf(stderr, "Erro: Comando mal executado...\n", argv[optind++]);
			exit(EXIT_FAILURE);
		}
		switch (c)
		{
		case 'u':
			// FUNCAO QUE VERIFICA SE ESTE NOME JÁ FOI INICIADO?
			if (strlen(optarg) != 0)
				strcpy(u->nome, optarg);
			break;
		default:
			break;
		}
		break;
	}
}

// ------------------------------------------------------------------------------------------------------
//                            .d8888. d88888b d8888b. db    db d88888b d8888b.
//                            88'  YP 88'     88  `8D 88    88 88'     88  `8D
//                            `8bo.   88ooooo 88oobY' Y8    8P 88ooooo 88oobY'
//                              `Y8b. 88~~~~~ 88`8b   `8b  d8' 88~~~~~ 88`8b
//                            db   8D 88.     88 `88.  `8bd8'  88.     88 `88.
//                            `8888Y' Y88888P 88   YD    YP    Y88888P 88   YD
// ------------------------------------------------------------------------------------------------------
void termina1()
{
	char inter_fifo_fname[20];

	for (int i = 0; i < MAXUSERS; i++)
	{
		sprintf(inter_fifo_fname, INTER_FIFO, i);
		unlink(inter_fifo_fname);
	}
	unlink(SERVER_FIFO_P);
	exit(0);
}

// Procura 'nome' recebido por parametro na base de dados recebida por parametro
int verifica_user(char *nome, server *s)
{
	char user[20];
	FILE *f = fopen(s->fich_nome, "r");
	if (f == NULL)
	{
		// se a base de dados recebida nao existir ou houver algum problema na sua abertura
		printf("\n Erro ao abrir a base de dados [%s]...n", s->fich_nome);
		return -1;
	}

	while (fscanf(f, " %[^\n]", user) == 1) // enquanto for possível ler nomes
	{
		if (strlen(user) > 8)
		{
			continue;
		}
		if (strcmp(user, nome) == 0) // os nomes são comparados
		{
			printf("\nUtilizador '%s' encontrado !\n", nome); // se forem iguais
			fclose(f);
			return 1;
		}
	}

	printf("\nUtilizador '%s' não encontrado...\n", nome); // se nao forem
	fclose(f);
	return 0;
}

// ------------------------------------------------------------------------------------------------------
// Mostra defenicoes

void mostra_def(editor t, server s)
{
	printf("\nNumero de linhas: %d\n", t.nlinhas);
	printf("Numero de colunas: %d\n", t.ncolunas);
	printf("Nome da base de dados: %s\n", s.fich_nome);
	printf("Numero de named pipes a utilizar: %d\n", s.n_named_pipes);
	printf("Nome do named pipe principal: %s\n", s.nome_pipe_p);
}

// Recebe opcao do servidor
void getOption_ser(int argc, char **argv, editor *t, user *u, server *s)
{

	int c;

	while (1)
	{
		// comandos
		static struct option long_options[] = {
			{"definicoes", no_argument, 0, 'd'},
			{"carregar", required_argument, 0, 'c'},
			{"guardar", required_argument, 0, 'g'},
			{"liberta", required_argument, 0, 'l'},
			{"ficheiro", required_argument, 0, 'f'},
			{"estatisticas", no_argument, 0, 'e'},
			{"utilizadores", no_argument, 0, 'u'},
			{"texto", no_argument, 0, 't'},
			{"help", no_argument, 0, 'h'},
			{"sair", no_argument, 0, 's'},
			{0, 0, 0, 0}};

		int option_index = 0;

		c = getopt_long(argc, argv, "dc:g:l:eutshp:n:f:", long_options, &option_index);

		if (c == -1) // se o getopt não receber nenhum comando
		{
			break;
		}

		if (optind < argc)
		{
			printf("[ERRO] Comando mal executado, consulte -help.", argv[optind++]);
			putchar('\n');
			exit(0);
		}

		// se receber, compara o comando recebido
		// quer pela long form ou pela short form
		// Exemplo: -d / --definicoes
		switch (c)
		{
		case 0:
			if (long_options[option_index].flag != 0)
				printf("Opcao %s", long_options[option_index].name);
			if (optarg)
				printf(" com o argumento %s", optarg);
			printf("\n");
			break;

		case 'd':
			mostra_def(*t, *s);
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

		case 'p':
			sprintf(SERVER_FIFO_P, "%s", optarg);
			// printf("O novo nome do pipe principal é : %s\n", s->nome_pipe_p);
			break;

		case 'n':
			printf("O numero de NPs agora é %d\n", atoi(optarg));
			break;

		case 'f':
			printf("\nBase de dados: %s\n", s->fich_nome);
			break;

		case '?':
			printf("\nConsulte -help para listar todos os comandos possiveis.\n");
			break;

		default:
			break;
		}
		break;
	}
}
// ------------------------------------------------------------------------------------------------------
void *verificaCliente(void *dados)
{
	valida val;
	int inter_pipes[MAXUSERS];
	int i;
	for (i = 0; i < MAXUSERS; i++)
		inter_pipes[i] = 0;
	server *ser = (server *)dados;
	int s_fifo_fd, val_fifo_fd, inter_fifo_fd;
	char val_fifo_fname[20], inter_fifo_fname[20], inter_fifo[20];

	int res, r, w, util = inter_pipes[0];

	// abre o npipe do servidor
	s_fifo_fd = open(SERVER_FIFO_P, O_RDWR);
	if (s_fifo_fd == -1)
	{
		perror("\nErro ao abrir o FIFO do servidor (RDWR/bloqueante)");
		exit(EXIT_FAILURE);
	}
	//	fprintf(stderr, "\nFIFO do servidor aberto para READ (+WRITE) bloqueante\n");

	//	printf("\nEstou preso  no read\n");
	// le a informação relativa à validacao do utilizador
	r = read(s_fifo_fd, &val, sizeof(val));
	// if (r == sizeof(valida))
	// 	fprintf(stderr, "\nRecebi %d bytes do user ...", r);

	// preenche o nome do pipe (val) com quem vai falar
	sprintf(val_fifo_fname, VAL_FIFO, val.pid_user);

	// abre o npipe de validacao para WR
	val_fifo_fd = open(val_fifo_fname, O_WRONLY);
	if (val_fifo_fd == -1)
	{
		perror("\nErro ao abrir o FIFO de valicao");
		close(s_fifo_fd);
		exit(EXIT_FAILURE);
	}

	// verifica se o username existe na base de dados
	// verifica_user > return 1 se existir
	val.ver = verifica_user(val.nome, ser);

	if (val.ver == 1)
	{
		for (i = 1; i < MAXUSERS; i++)
		{
			if (inter_pipes[i] < util)
			{
				util = inter_pipes[i];
				pos = i;
			}
		}
		inter_pipes[pos] += 1;
		// pipe para onde o cliente passa a falar
		sprintf(inter_fifo_fname, INTER_FIFO, pos);

		for (i = 0; i < MAXUSERS; i++)
		{
			sprintf(inter_fifo, INTER_FIFO, i);

			if (mkfifo(inter_fifo, 0600) == -1)
			{
				fprintf(stderr, "\nErro ao abrir o pipe 'sss%d' de interação do servidor", i);
				close(val_fifo_fd);
				unlink(val_fifo_fname);
				exit(EXIT_FAILURE);
			}
		}
	}

	// copiar o nome do novo pipe para onde
	// o cliente vai falar
	strcpy(val.np_name, inter_fifo_fname);
	//	printf("\nfifo onde recebo o cliente: %s", val.np_name); fflush(stdout);
	// escreve a informação da validacao no npipe
	// de validacao
	w = write(val_fifo_fd, &val, sizeof(val));
	// if ( w == sizeof(val))
	// 	fprintf(stderr,"\nEnviei [%d bytes] ao user",w);

	// fecha o pipe das validações
	close(val_fifo_fd);
	pthread_exit(0);

	return NULL;
}
// ------------------------------------------------------------------------------------------------------

void *serv_cli(void *dados)
{
	int editores[MAXLINES];
	comunica com;
	int r, w, i, c_fifo_fd;
	char c_fifo_fname[20];

	// inicializar o vetor com 0s
	for (i = 0; i < MAXLINES; i++)
		editores[i] = 0;

	// feito de forma errada
	char inter_fifo_fname[20];

	sprintf(inter_fifo_fname, INTER_FIFO, pos);
	//printf("inter name: %s", inter_fifo_fname);

	inter_fifo_fd = open(inter_fifo_fname, O_RDWR);
	if (inter_fifo_fd == -1)
	{
		fprintf(stderr, "\n O pipe interacao nao abriu\n");
		close(s_fifo_fd);
		exit(EXIT_FAILURE);
	}
	else
		// printf("\ninter fifo aberto para rd+wr\n");
		// printf("\nSAIR? %d\n", SAIR);

		while (SAIR == 0)
		{
			// le do pipe de interaçao
			printf("\nestou bloqueado no read\n");

			r = read(inter_fifo_fd, &com.request, sizeof(com.request));
			if (r < sizeof(com.request))
			{
				fprintf(stderr, "\nRecebido um request incompleto"
								"\n[bytes lidos: %d]",
						r);
				continue;
			}

			printf("\n> Numero de linha : [%d]\n", com.request.nr_linha);

			if (com.request.aspell == 0)
			{
				printf("\nLINHA ATUAL: %s\n", com.request.texto);
				com.controlo.sair = SAIR;

				// verifica se a linha está livre...
				for (i = 0; i < MAXLINES; i++)
				{
					if (i == com.request.nr_linha)
					{
						if (editores[i] == com.request.pid_cliente)
						{
							editores[i] = 0;
							break;
						}
						if (editores[i] == 0)
						{ // linha esta livre
							editores[i] = com.request.pid_cliente;
							com.controlo.perm = 1;
						}
						else // linha nao esta livre
							com.controlo.perm = 0;
					}
				}

				for (i = 0; i < MAXLINES; i++)
				{
					printf("\tLINHA: %d", i);
					printf("\tEDITOR:%d\n", editores[i]);
					fflush(stdout);
				}
			}
			else if (com.request.aspell == 1)
			{
				//Chama a função de verificação
				dicionario(&com);
			}

			// OBTEM O NOME DO FIFO PARA ONDE VAI RESPONDER
			sprintf(c_fifo_fname, CLIENT_FIFO, com.request.pid_cliente);

			c_fifo_fd = open(c_fifo_fname, O_WRONLY);
			if (c_fifo_fd == -1)
				perror("\nErro no open - Ninguem quis a resposta");
			else
			{
				// fprintf(stderr, "\nFIFO cliente aberto para WRITE");

				w = write(c_fifo_fd, &com.controlo, sizeof(com.controlo));
				// if( w == sizeof(com.controlo) )
				// fprintf(stderr,"\nescreveu a resposta");
				if (w != sizeof(com.controlo))
					perror("\nerro a escrever a resposta");

				close(c_fifo_fd);
				// fprintf(stderr,"\nFIFO cliente fechado");
			}
		}
	pthread_exit(0);
	return NULL;
}
// ------------------------------------------------------------------------------------------------------
void banner()
{
	system("clear");
	printf("\n\n\n.d8888. d88888b d8888b. db    db d88888b d8888b. \n");
	printf("88'  YP 88'     88  `8D 88    88 88'     88  `8D  \n");
	printf("`8bo.   88ooooo 88oobY' Y8    8P 88ooooo 88oobY'  \n");
	printf("  `Y8b. 88~~~~~ 88`8b   `8b  d8' 88~~~~~ 88`8b   \n");
	printf("db   8D 88.     88 `88.  `8bd8'  88.     88 `88. \n");
	printf("`8888Y' Y88888P 88   YD    YP    Y88888P 88   YD  \n");
}
// ------------------------------------------------------------------------------------------------------
void commandline(editor *edit, server *ser)
{

	char comando[50];
	char cmd[20], argumento[30];
	int op;
	do
	{
		system("clear");
		banner();
		printf("\nserver%@admin$:", getpid());
		fflush(stdout);
		scanf(" %[^\n]", comando);
		sscanf(comando, " %19s %29[^\n]", cmd, argumento);

		if (!strcmp(cmd, "settings"))
		{
			mostra_def(*edit, *ser);
		}
		else if (!strcmp(cmd, "load"))
		{ // tem argumento
			printf("sou o load\n");
		}
		else if (!strcmp(cmd, "save"))
		{ // tem argumento
			printf("sou o save\n");
		}
		else if (!strcmp(cmd, "free"))
		{ // tem argumento
			printf("sou o free\n");
		}
		else if (!strcmp(cmd, "statistics"))
		{
			printf("eu sou statistics");
		}
		else if (!strcmp(cmd, "users"))
		{
			printf("eu sou o users\n");
		}
		else if (!strcmp(cmd, "text"))
		{
			printf("eu sou o text\n");
		}
		else if (!strcmp(cmd, "shutdown"))
		{
			SAIR = 1;
			//printf("eu sou o shutdown");
			kill(getpid(), SIGUSR1);
		}
		else if (!strcmp(cmd, "help"))
		{
			printf(" > Comando disponiveis:\n\tsettings\n\tsave <filename>\n\tfree <linenumber>\n\tstatistics\n\tusers\n\ttext\n\tshutdown\n");
		}
		else
			printf("O comando '%s' nao existe, execute 'help' para listar os comandos disponiveis\n", cmd);

		sleep(2);

		// if(!strcmp(cmd, "load") || !strcmp(cmd, "save") || !strcmp(cmd, "free"))
		// 	printf("Comando: %s argumento: %s\n", cmd, argumento);
		// else
		// 	printf("Comando: %s\n", cmd);

	} while (strcmp(cmd, "shutdown") != 0);
}

void dicionario(comunica *original)
{
	int ida[2], volta[2], r, i, conta_enter = 0;
	char total[400], frase[MAXCOLUMNS + 1], *aux, pos1;
	pid_t processo;

	if (pipe(ida) < 0)
	{
		printf("!!!!ERRO AO CRIAR O PIPE IDA!!!");
		exit(EXIT_FAILURE);
	}
	if (pipe(volta) < 0)
	{
		printf("!!!!ERRO AO CRIAR O PIPE VOLTA!!!");
		exit(EXIT_FAILURE);
	}

	if ((processo = fork()) == 0)
	{
		// fazer do stdin a saida do pipe ida
		if (dup2(ida[0], 0) < 0)
		{
			fprintf(stderr, "\nErro ao mudar os descritores stdin!!\n");
			exit(EXIT_FAILURE);
		}
		//fecha as entradas do pipe
		close(ida[0]);
		close(ida[1]);
		// fazer do stdout a entrada do pipe volta
		if (dup2(volta[1], 1) < 0)
		{
			fprintf(stderr, "\nErro ao mudar os descritores stdout!!\n");
			exit(EXIT_FAILURE);
		}
		//fecha as entradas do pipe
		close(volta[1]);
		close(volta[0]);

		//fprintf(stderr, "\nFILHO DO DICIONARIO!!!\n");

		//executa o aspell
		execlp("aspell", "aspell", "-a", "-l", "pt_PT", NULL);

		exit(EXIT_SUCCESS);
	}

	else if (processo < 0)
	{ //se der erro
		fprintf(stderr, "\nErro ao fazer o fork!\n");
		exit(EXIT_FAILURE);
	}
	else
	{ // se for pai
		//fprintf(stderr, "\nPAI DO DICIONARIO!!!\n");
		r = read(volta[0], &total, sizeof(char) * 400);
		//fprintf(stderr, "\nFilhos é só dor de cabeça...\n");

		if (r >= 0)
		{
			total[r] = '\0';
		}
		else
		{
			fprintf(stderr, "Erro na leitura do inicio do aspell\n");
		}

		for (i = 0; i < MAXCOLUMNS; i++)
		{
			original->controlo.texto_certo[i] = ' ';
		}

		strcpy(frase, original->request.texto);

		fprintf(stderr, "\nString inicial: '%s'\n", original->request.texto);
		i = 0;
		//Ciclo que divide as palavras
		aux = (char *)strtok(frase, " ");
		frase[MAXCOLUMNS] = '\0';

		while (aux != NULL)
		{
			strcpy(total, aux);
			fprintf(stderr, "\nString depois do strtok: %s!!", total);
			write(ida[1], &total, strlen(total));

			//Validação do enter
			write(ida[1], "\n", sizeof(char));

			r = read(volta[0], &total, sizeof(char) * 400);

			pos1 = total[0];

			fprintf(stderr, "\nOutput do aspell: '%s'!!\n", total);

			// Se TOTAL tive dois '\n' ENTAO recebi tudo OK!
			//                         SENAO tenho de fazer mais READS!
			if (r > 0)
			{
				if (total[r - 1] == '\n' && total[r] == '\n')
				{
					total[r] = '\0';
				}
				else
				{
					read(volta[0], &total, sizeof(char) * 400);
				}
			}
			else
			{
				printf("Erro na leitura do output aspell\n");
			}

			if (pos1 == '*' || pos1 == '+')
			{
				original->controlo.texto_certo[i] = '*';
				i++;
			}
			else if (pos1 == '&')
			{
				original->controlo.texto_certo[i] = '&';
				i++;
			}

			aux = (char *)strtok(NULL, " ");
		}
	}
}