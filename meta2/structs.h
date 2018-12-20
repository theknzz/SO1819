#ifndef __STRUCTS__
#define __STRUCTS__

// libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
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

typedef struct Server {
    int n_utilizadores, n_utilizadores_max, n_named_pipes, fich_utilizadores;
    char fich_nome[10], chars_mais_comuns[5], nome_pipe_p[10];
} server;

typedef struct User {
    char nome[9];
    int tempo_linha, linhas_editadas, linha_atual;
} user;

typedef struct Avisos {
    int tipo;
    char aviso[50];
} avisos;

typedef struct Editor {
    int nlinhas, ncolunas, l_atual, c_atual, n_palavras, n_letras;
    char tab[MAXLINES][MAXCOLUMNS];
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
} comunica;

#endif