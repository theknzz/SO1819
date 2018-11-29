#include "server.h"
#include "structs.h"

int main( int argc, char **argv) {

	editor t;
	user u;
	server s;
	int editores[MAXLINES], inter_pipes[MAXUSERS];

	// descritores dos pipes
	int s_fifo_fd, c_fifo_fd, r, w, val_fifo_fd, inter_fifo_fd;
	char c_fifo_fname[20], val_fifo_fname[20], inter_fifo_fname[20];
	// strcuts
	request req;
	controlo ctrl;
	valida val;
	// variaveis controlo
	int cfm, res , i;

	int util = inter_pipes[0];
	int pos = 0;

	// inicia variaveis
	// inicia_vars(&t, &u, &s);
	// comandos adicionais do servidor
	getOption_ser(argc, argv, &t, &u, &s);

	// verfica se já existe servidor
	if (access(SERVER_FIFO_P, F_OK) == 0)
	{
		fprintf(stderr, "\n Já existe um servidor!\n");
		exit(EXIT_FAILURE);
	}

	// inicializar o vetor com -1s
	for (i = 0; i < MAXLINES; i++)
	{
		editores[i] = -1;
	}

	// inicializar o vetor com 0s
	for (i = 0; i < MAXUSERS; i++)
	{
		inter_pipes[i] = 0;
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

	// preenche o nome do pipe (val) com quem vai falar
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
	// ERRO na abertura do ficheiro s->fich_nome (tem problemas)
	val.ver = verifica_user(val.nome, &s);

	// NAO PODE SER FEITO COM UM FOR PORQUE
	// SENAO DEPOIS NAO CONSIGO DAR UNLINK
	for(i = 0; i < MAXUSERS ; i++)
	{
		sprintf(inter_fifo_fname, INTER_FIFO, i);		
		res = mkfifo(inter_fifo_fname, 0600);
		if(res == -1)
		{
			fprintf(stderr,"\nErro ao abrir o pipe 'sss%d' de interação do servidor", i);
			exit(EXIT_FAILURE);
		}
	}

	// FALTA FAZER ...
	// named pipe para onde o cliente passa a falar

	if(val.ver == 1)
	{	
		for (i=1 ; i < MAXUSERS; i++)
		{
			if(inter_pipes[i] < util) {
				util = inter_pipes[i];
				pos = i;
			}
		}
		inter_pipes[pos] += 1;
		sprintf(inter_fifo_fname, INTER_FIFO, pos);
	}
	inter_fifo_fd = open(inter_fifo_fname, O_RDWR);
	if(inter_fifo_fd == -1)
	{
		fprintf(stderr,"\n O pipe val nao abriu\n");
		close(s_fifo_fd);
		close(val_fifo_fd);
		exit(EXIT_FAILURE);
	}
	// copiar o nome do novo pipe para onde
	// o cliente vai falar
	strcpy(val.np_name, inter_fifo_fname);
	
	// escreve a informação da validacao no npipe
	// de validacao
	w = write(val_fifo_fd, &val, sizeof(val));
	if ( w == sizeof(val))
		fprintf(stderr,"\nEnviei [%d bytes] ao cliente",w);
	
	// fecha o pipe das validações
	close(val_fifo_fd); 

	while(1) {
		// le do pipe de interaçao
		r = read(inter_fifo_fd, &req, sizeof(req));
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

	return 0;
}