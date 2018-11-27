#include "server.h"
#include "structs.h"

int main( int argc, char **argv) {

	editor t;
	user u;
	server s;
	int verify = 0, res, i;
	int editores[MAXLINES];
		int pos_editor = -1, pos_livre = -1;
	for (i = 0; i < MAXLINES; i++)
	{
		editores[i] = -1;
	}

	// descritores dos pipes
	int s_fifo_fd, c_fifo_fd, r, w;
	char c_fifo_fname[10];
	request req;
	controlo ctrl;

	res = mkfifo(SERVER_FIFO_P, 0600);
	if( res == -1 ) {
		perror("\nmkfifo do FIFO do servidor deu erro");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "\nFIFO servidor criado");

	s_fifo_fd = open(SERVER_FIFO_P, O_RDWR);
	if(s_fifo_fd == -1) {
		perror("\nErro ao abrir o FIFO do servidor (RDWR/bloqueante)");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "\nFIFO do servidor aberto para READ (+WRITE) bloqueante\n");

	while(1) {
		r = read(s_fifo_fd, &req, sizeof(req));
		if(r < sizeof(req)){
			fprintf(stderr,"\nRecebido um request incompleto"
							"\n[bytes lidos: %d]", r);
			continue;
		}
		fprintf(stderr, "\n\t> Numero de linha : [%d]", req.nr_linha);

		// VERIFICA SE A LINHA ESTA LIVRE ...
		if(ctrl.perm == 1){
			for (i = 0; i < MAXLINES; i++)
					if(editores[i] == req.pid_cliente)
						editores[i] = -1;
					ctrl.perm = 0; // nao tem permissao para editar a linha
		}
		else {
			int pos_editor = -1, pos_livre = -1;
			for(i = 0; i < MAXLINES; i++)
			{
				if(editores[i] == -1 && pos_livre == -1)
					pos_livre = i;

				if(editores[i] == req.pid_cliente) {
					pos_editor = i;
				}
			}
			if( pos_editor == -1 && pos_livre != -1) {
				editores[pos_livre] = req.pid_cliente;
				ctrl.perm = 1; // tens permissao para editar a linha
			}
		}
		for(i = 0 ; i<MAXLINES; i++)
		{
			printf("\nLINHA: %d \tEDITOR:%d", i + 1, editores[i]);
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