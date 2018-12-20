#include "server.h"
#include "structs.h"

#define NTHR 1
#define NMTX 3

int main(int argc, char **argv) {

	char 	inter_fifo_fname[20];
	int 	i;
	editor	t;
	user 	u;
	server 	s;
	char 	str[20];
	
	// inicia variaveis
	inicia_vars(&t, &u, &s);

	// comandos adicionais do servidor
	getOption_ser(argc, argv, &t, &u, &s);

	// threads
	pthread_t tarefa[NTHR];
	pthread_mutex_init(&trinco, NULL);

	// signal
	signal(SIGUSR1, termina1);

	banner();

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

	pthread_create(&tarefa[0], NULL, verificaCliente, &s);

	serv_cli(&s);

	// pthread_create(&tarefa[1], NULL, serv_cli, NULL);

	commandline(&t, &s);

	
	pthread_join(tarefa[0], NULL);
	// pthread_join(tarefa[1], NULL);
	pthread_mutex_destroy(&trinco);

	exit(EXIT_SUCCESS);
}