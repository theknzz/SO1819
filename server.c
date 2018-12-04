#include "server.h"
#include "structs.h"

int main(int argc, char **argv) {

	char inter_fifo_fname[20];
	int i;

	char str[20];
	banner();
	// inicia variaveis
	inicia_vars(&t, &u, &s);

	// comandos adicionais do servidor
	getOption_ser(argc, argv, &t, &u, &s);

	// threads
	pthread_t tarefa[2];
	
	signal(SIGUSR1, termina1);

	// verfica se já existe servidor
	if (access(SERVER_FIFO_P, F_OK) == 0)
	{
		fprintf(stderr, "\n Já existe um servidor!\n");
		exit(EXIT_FAILURE);
	}

	// cria o npipe do servidor
	if(mkfifo(SERVER_FIFO_P, 0600) == -1){
		perror("\nmkfifo do FIFO do servidor deu erro");
		exit(EXIT_FAILURE);
	}

	SAIR = 0;	
	pthread_create(&tarefa[0], NULL, verificaCliente, &val);
	pthread_join(tarefa[0], NULL);

	pthread_create(&tarefa[1], NULL, serv_cli, NULL);

	commandline();

	//SAIR = 1;
	pthread_join(tarefa[1], NULL);
	close(s_fifo_fd);
	close(inter_fifo_fd);
	for(i = 0; i < MAXUSERS; i++)	{
		sprintf(inter_fifo_fname, INTER_FIFO, i);
		unlink(inter_fifo_fname);
	}
	unlink(SERVER_FIFO_P);

	exit(EXIT_SUCCESS);

}