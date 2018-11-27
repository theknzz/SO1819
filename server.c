#include "server.h"
#include "structs.h"

int main(int argc, char **argv)
{

	editor t;
	user u;
	server s;
	int verify = 0, fd_ser;

	if (access(SERVER_FIFO_P, F_OK) == 0)
	{
		fprintf(stderr, "\n Já existe um servidor!\n");
		return -1;
	}

	mkfifo(SERVER_FIFO_P, 0600);

	if ((fd_ser = open(SERVER_FIFO_P, O_RDWR)) == -1)
	{
		fprintf(stderr, "\nO pipe não abriu!\n");
		return -1;
	}

	strcpy(s.nome_pipe_p, SERVER_FIFO_P);

	// problema : até que ponto é que faz sentido um utilizador executar um comando errado e
	// só depois de iniciar sessão é que recebe o aviso que o comando não funciona?
	printf("\nInsira a base de dados: \n");
	scanf(" %9[^\n]", s.fich_nome);
	system("clear");

	read(fd_ser, &u, sizeof(u));

	verify = verifica_user(u.nome, s.fich_nome);

	if (verify == 1)
	{
		inicia_vars(&t, &u, &s);
		getOption(argc, argv, &t, &u, &s);
	}
	else
		printf("\nUtilizador '%s' não tem acesso a esse comando.\n", u.nome);

	close(fd_ser);
	unlink(SERVER_FIFO_P);
	return 0;
}