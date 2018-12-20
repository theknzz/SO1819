#include "structs.h"
#include "client.h"

int main(int argc, char **argv)
{
    editor t;
    user u;
    server s;
    WINDOW *janela;
    
    char val_fifo_fname[20], inter_fifo_fname[20];
    valida val;
    int val_fifo_fd, res, s_fifo_fd, r, w;

    // Verificar se o NP Servidor existe
    if(access(SERVER_FIFO_P, F_OK) != 0) {
        fprintf(stderr, "\nServidor não está iniciado...!\n");
        exit(EXIT_FAILURE);
    }
    // abre o np do servidor
    s_fifo_fd = open(SERVER_FIFO_P, O_WRONLY);
    if(s_fifo_fd == -1) {
        fprintf(stderr, "\nO pipe principal do server não abriu!\n");
        exit(EXIT_FAILURE);
    }

    // nome que do np que vai ser aberto
    // para verificar o user
    // VAL_FIFO("vvv") + pid do user
    sprintf(val_fifo_fname, VAL_FIFO, getpid());

    // cria o pipe de validacao
    if(mkfifo(val_fifo_fname, 0600) == -1) {
		perror("\nmkfifo do FIFO do validacao deu erro");
		exit(EXIT_FAILURE);
	}
	//fprintf(stderr, "\nFIFO validacao criado\n");
    // abre o pipe de validcao

    val_fifo_fd = open(val_fifo_fname, O_RDWR);
    if(val_fifo_fd == -1)
    {
       fprintf(stderr,"\n O pipe val nao abriu\n");
       close(s_fifo_fd);
       exit(EXIT_FAILURE);
    }

    getOption_cli(argc, argv, &u);
    
    if(strlen(argv[1])==0)
    {
        // preenche o nome do user
    	printf("\nInsira o nome de utilizador: ");
	    scanf(" %9[^\n]", val.nome);
    }
    else
        strcpy(val.nome, u.nome);

    // preenche a estrutura com o pid do user
    val.pid_user = getpid();

    // manda a informacao ao servidor
    // para ser validada
    w = write(s_fifo_fd, &val, sizeof(val));
    if (w == sizeof(val))
        fprintf(stderr, "\nEnviei [%d bytes]...", w);


    // le a informação recebida do servidor
    // validation info
    r = read(val_fifo_fd, &val, sizeof(val));
    if( r == sizeof(val))
        fprintf(stderr,"\nli de val [%d bytes]", r);

    //printf("\nfifo para onde falo: %s", val.np_name); fflush(stdout);
    close(val_fifo_fd);
    unlink(val_fifo_fname);

    // ----------- USER PASSA A SER UM CLIENTE  ----------- 
    // se o utiizador for cliente
    if(val.ver == 1)
    {
        t.tab[t.nlinhas][t.ncolunas];
        strcpy(inter_fifo_fname, val.np_name);
        printf("NOME: %s", inter_fifo_fname);
        scanf("%d", &w);
        inicia_vars(&t, &u, &s);
        strcpy(u.nome, val.nome);
        criar_editor(janela, &t, t.tab, inter_fifo_fname);
    }
    else if (val.ver == 0)
        fprintf(stderr,"\n '%s' nao consta na base de dados\n", val.nome);
    else if (val.ver == 2)
        fprintf(stderr,"\nLimite maximo de clientes foi atingido.\n");
    else
        fprintf(stderr,"\nErro ao abrir a base de dados\n");

    return 0;
}