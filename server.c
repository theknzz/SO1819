#include "server.h"
#include "structs.h"

#define NMTX 1

int main(int argc, char **argv) {

	char 	inter_fifo_fname[20];
	editor	t;
	user 	u;
	char 	str[20];
	container box;
	
	int editores[MAXLINES], r, w, i, c_fifo_fd;
	comunica com;
	informacao *info;
	char c_fifo_fname[20];

	pthread_t *lenp;

	// threads
	pthread_t tarefa;

	// inicia variaveis
	inicia_vars(&t, &u, &box.server);

	// comandos adicionais do servidor
	getOption_ser(argc, argv, &t, &u, &box.server);

	trinco = (pthread_mutex_t *) malloc (NMTX * sizeof(pthread_mutex_t));
	lenp = (pthread_t *)malloc(nr_np * sizeof(pthread_t));
	info = (informacao *)malloc(nr_np * sizeof(informacao));
	if(lenp == NULL || info == NULL || trinco == NULL) {
		fprintf(stderr, "erro na alocacao da memoria\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i < NMTX; i++)
		pthread_mutex_init(&trinco[i], NULL);

	// signal
	signal(SIGUSR1, termina1);

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

	pthread_create(&tarefa, NULL, verificaCliente, &box);

	// inicializar o vetor com 0s
	for (i = 0; i < MAXLINES; i++)
	{
		editores[i] = 0;
	}

	for (i = 0; i < nr_np; i++)
	{
		info[i].num = i;
		printf("Num: %d\n", i);
		pthread_create(&lenp[i], NULL, employee, &info[i]);
	}

	commandline(&t, &box, &t);

	for (i = 0; i < nr_np; i++)
		pthread_join(lenp[i], NULL);
	pthread_join(tarefa, NULL);

	for(i=0;i<NMTX;i++)
		pthread_mutex_destroy(&trinco[i]);

	exit(EXIT_SUCCESS);
}