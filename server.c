#include "server.h"
#include "structs.h"

int main( int argc, char **argv) {

	editor t;
	user u;
	server s;
	int verify = 0, res, i;
	int editores[MAXLINES];

	// descritores dos pipes
	int s_fifo_fd, c_fifo_fd, r, w, val_fifo_fd;
	char c_fifo_fname[20], val_fifo_fname[20];
	request req;
	controlo ctrl;
	valida val;
	int cfm;

	if (access(SERVER_FIFO_P, F_OK) == 0)
	{
		fprintf(stderr, "\n Já existe um servidor!\n");
		exit(EXIT_FAILURE);
	}

	//mkfifo(SERVER_FIFO_P, 0600);

	// if ((s_fifo_fd = open(SERVER_FIFO_P, O_RDWR)) == -1)
	// {
	// 	fprintf(stderr, "\nO pipe não abriu!\n");
	// 	exit(EXIT_FAILURE);
	// }

	// inicializar o vetor com -1
	for (i = 0; i < MAXLINES; i++)
	{
		editores[i] = -1;
	}

	// cria o npipe do servidor
	res = mkfifo(SERVER_FIFO_P, 0600);
	if( res == -1 ) {
		perror("\nmkfifo do FIFO do servidor deu erro");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "\nFIFO servidor criado");

	// abre o npipe do servidor
	s_fifo_fd = open(SERVER_FIFO_P, O_RDWR);
	if(s_fifo_fd == -1) {
		perror("\nErro ao abrir o FIFO do servidor (RDWR/bloqueante)");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "\nFIFO do servidor aberto para READ (+WRITE) bloqueante\n");

	// le a informação relativa à validacao do utilizador
	r = read(s_fifo_fd, &val, sizeof(val));
	if (r == sizeof(val))
		fprintf(stderr, "\nRecebi %d bytes do cliente ...", r);

	sprintf(val_fifo_fname, VAL_FIFO, val.pid_user);
	// abre o npipe de validacao para WR
	val_fifo_fd = open(val_fifo_fname, O_WRONLY);
	if(val_fifo_fd == -1)
	{
		perror("\nErro ao abrir o FIFO de valicao");
		close(s_fifo_fd);
		exit(EXIT_FAILURE);
	}

	// verifica se o username existe na base de dados
	// verifica_user > return 1 se existir 
	val.ver = verifica_user(val.nome, &s);

	// escreve a informação da validacao no npipe
	// de validacao
	w = write(val_fifo_fd, &val, sizeof(val));
	if ( w == sizeof(val))
		fprintf(stderr,"\nEnviei [%d bytes] ao cliente",w);
	
	//close(val_fifo_fd); 

	while(1) {
		r = read(s_fifo_fd, &req, sizeof(req));
		if(r < sizeof(req)){
			fprintf(stderr,"\nRecebido um request incompleto"
							"\n[bytes lidos: %d]", r);
			continue;
		}
		fprintf(stderr, "\n> Numero de linha : [%d]\n", req.nr_linha);
		
		// VERIFICA SE A LINHA ESTA LIVRE ...	
		for(i = 0; i<MAXLINES; i++){
			
			if(i == req.nr_linha) {

			if(editores[i] == req.pid_cliente)
			{
				editores[i] = -1;
				break;
			}
				if(editores[i] == -1)
				{ // linha esta livre
					editores[i] = req.pid_cliente;
					ctrl.perm = 1;
				}
				else // linha nao esta livre
					ctrl.perm = 0;
			}
		}

		for(i = 0 ; i<MAXLINES; i++)
		{
			printf("\tLINHA: %d", i+1);
			printf("\tEDITOR:%d\n", editores[i]);
		}

		// FALTA FAZER ...
		strcpy(ctrl.np_name, "test"); // named pipe para onde o cliente passa a falar

		// OBTEM O NOME DO FIFO PARA ONDE VAI RESPONDER
		sprintf(c_fifo_fname, CLIENT_FIFO, req.pid_cliente);

		c_fifo_fd = open(c_fifo_fname, O_WRONLY);
		if(c_fifo_fd == -1)
			perror("\nErro no open - Ninguem quis a resposta");
		else {
				fprintf(stderr, "\nFIFO cliente aberto para WRITE");

				w = write(c_fifo_fd, &ctrl, sizeof(ctrl));
				if( w == sizeof(ctrl) )
					fprintf(stderr,"\nescreveu a resposta");
				else
					perror("\nerro a escrever a resposta");

				close(c_fifo_fd);
				fprintf(stderr,"\nFIFO cliente fechado");
			}
	}
	close(s_fifo_fd);
	unlink(SERVER_FIFO_P);
	
	// inicia_vars(&t,&u,&s);
	// getOption(argc, argv, &t, &u, &s);
	return 0;
}