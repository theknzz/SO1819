#include "server.h"
#include "structs.h"
#include <signal.h>

#define NMTX 1

int main(int argc, char **argv) {

	char 	inter_fifo_fname[20];
	editor	t;
	user 	u;
	server 	s;
	char 	str[20];
	
	int editores[MAXLINES], r, w, i, j, c_fifo_fd;
	comunica com;
	informacao *info;
	char c_fifo_fname[20];
	struct sigaction act;
	// container box;

	pthread_t *lenp;

	// threads
	pthread_t tarefa;

	// inicia variaveis
	inicia_vars(&t, &u, &s);

	s.tab = (char **) malloc(sizeof(char *) * t.nlinhas);
	if(s.tab == NULL){
		fprintf(stderr, "\nERRO AO ALOCAR MEMORIA PARA A TABELA!\n");
		exit(EXIT_FAILURE);
	}
	for(i = 0; i < t.nlinhas; i++){
		s.tab[i] = (char *) malloc (sizeof(char) * t.ncolunas);
		if(s.tab[i] == NULL){
			fprintf(stderr, "\nERRO AO ALOCAR MEMORIA PARA A TABELA!\n");
			exit(EXIT_FAILURE);
		}
		for(j = 0; j < t.ncolunas; j++){
			s.tab[i][j] = ' ';
		}
	}

	s.linhas = t.nlinhas;
	s.colunas = t.ncolunas;

	// comandos adicionais do servidor
	getOption_ser(argc, argv, &t, &u, &s);

	users = (user *) malloc(s.n_utilizadores_max * sizeof(user));
	trinco = (pthread_mutex_t *) malloc (NMTX * sizeof(pthread_mutex_t));
	lenp = (pthread_t *)malloc(nr_np * sizeof(pthread_t));
	info = (informacao *)malloc(nr_np * sizeof(informacao));
	if(lenp == NULL || info == NULL || trinco == NULL || users == NULL) {
		fprintf(stderr, "erro na alocacao da memoria\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < NMTX; i++)
		pthread_mutex_init(&trinco[i], NULL);

	// signals
	signal(SIGUSR1, termina1);
	act.sa_sigaction = sai_cli;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGTERM, &act, NULL);

	// verfica se já existe servidor
	if (access(SERVER_FIFO_P, F_OK) == 0)
	{
		fprintf(stderr, "\n Já existe um servidor!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		cria_np_interacao();
	}
	
	// cria o npipe do servidor
	if (mkfifo(SERVER_FIFO_P, 0600) == -1)
	{
		perror("\nmkfifo do FIFO do servidor deu erro");
		exit(EXIT_FAILURE);
	}

	SAIR = 0;

	pthread_create(&tarefa, NULL, verificaCliente, &s);

	// inicializar o vetor com 0s
	for (i = 0; i < MAXLINES; i++)
	{
		editores[i] = 0;
	}

	for (i = 0; i < nr_np; i++)
	{
		info[i].s = &s;
		info[i].num = i;
//		printf("Num: %d\n", info[i].num);
		pthread_create(&lenp[i], NULL, employee, &info[i]);
	}

	//scanf("%d", &i);
	commandline(&s, &t);

	for (i = 0; i < nr_np; i++)
		pthread_join(lenp[i], NULL);

	pthread_join(tarefa, NULL);

	for(i=0;i<NMTX;i++)
		pthread_mutex_destroy(&trinco[i]);

	for(i = 0; i < t.nlinhas; i++){
		free(s.tab[i]);
	}

	free(s.tab);
	exit(EXIT_SUCCESS);
}