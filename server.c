#include "server.h"
#include "structs.h"

#define NTHR 1
#define NMTX 3

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

	lenp = (pthread_t *)malloc(MAXUSERS * sizeof(pthread_t));
	info = (informacao *)malloc(MAXUSERS * sizeof(informacao));
	if(lenp == NULL || info == NULL) {
		fprintf(stderr, "erro na alocacao da memeoria\n");
		exit(EXIT_FAILURE);
	}

	// threads
	pthread_t tarefa[NTHR];
	pthread_mutex_init(&trinco, NULL);

	// inicia variaveis
	inicia_vars(&t, &u, &box.server);

	// comandos adicionais do servidor
	getOption_ser(argc, argv, &t, &u, &box.server);

	printf("\nBase de dados: %s.......\n", box.server.fich_nome);
	sleep(2);

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

	pthread_create(&tarefa[0], NULL, verificaCliente, &box);

	// inicializar o vetor com 0s
	for (i = 0; i < MAXLINES; i++)
	{
		editores[i] = 0;
	}

	for (i = 0; i < MAXUSERS; i++)
	{
		info[i].num = i;
		printf("Num: %d\n", i);
		pthread_create(&lenp[i], NULL, employee, &info[i]);
	}

	commandline(&t, &box);

	for (i = 0; i < MAXUSERS; i++)
		pthread_join(lenp[i], NULL);
	pthread_join(tarefa[0], NULL);
	// pthread_join(tarefa[1], NULL);
	pthread_mutex_destroy(&trinco);

	exit(EXIT_SUCCESS);
}