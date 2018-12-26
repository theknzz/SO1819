#ifndef __STRUCTS__
#define __STRUCTS__

// libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <curses.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <pthread.h>
#include <fcntl.h>

// defines
#define MAXLINES 15
#define MAXCOLUMNS 45
#define TIME_OUT 10
#define MAXUSERS 3

// comunicação
#define SERVER_FIFO_P "sss"
#define CLIENT_FIFO "ccc%d"
#define VAL_FIFO "vvv%d"
#define INTER_FIFO "sss%d"

// vars
int SAIR;


typedef struct Server {
    int n_utilizadores, n_utilizadores_max, n_named_pipes, fich_utilizadores, linhas, colunas;
    char fich_nome[10], chars_mais_comuns[5], nome_pipe_p[10], **tab;
} server;

typedef struct User {
    char nome[9];
    char nome_np_inter[20];
    float linhas_escritas;
    int linha_atual;
    pid_t user_pid;
} user;

typedef struct Avisos {
    int tipo;
    char aviso[50];
} aviso;

typedef struct Editor {
    int nlinhas, ncolunas, l_atual, c_atual, n_palavras, n_letras, tempo_max_linha;
} editor;

typedef struct Valida {
    //char file[10];
    pid_t pid_user;
    int ver;
    char nome[9];
    char np_name[20];
} valida;

struct Request {
    pid_t pid_cliente;
    int nr_linha, aspell;
    char texto[MAXCOLUMNS];
};

struct Controlo {
    int perm; // permissao para editar linha
    int sair; // servidor vai fechar?
    char texto_certo[MAXCOLUMNS];
};

typedef struct Comunica {
    struct Request request;
    struct Controlo controlo;
    pid_t server_pid;
} comunica;

typedef struct Informacao {
    int num;
    server *s;
} informacao;


// typedef struct Container {
//     struct  User users[MAXUSERS];
//     struct  Server server;
//     int num;
// } container;


#endif