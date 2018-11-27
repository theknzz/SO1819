#ifndef STRUCTS
#define STRUCTS

// libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <getopt.h>
#include <curses.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

// defines
#define MAXLINES 15
#define MAXCOLUMNS 45
#define TIME_OUT 10
#define MAXUSERS 3

// comunicação
#define SERVER_FIFO_P "sss"
#define NR_NP 2
#define CLIENT_FIFO "ccc%d"

typedef struct Server {
    int n_utilizadores, n_utilizadores_max, n_named_pipes, fich_utilizadores;
    char fich_nome[10], chars_mais_comuns[5], nome_pipe_p[10];
} server;

typedef struct User {
    char nome[8];
    int tempo_linha, linhas_editadas, linha_atual;
} user;

typedef struct Avisos {
    int tipo;
    char aviso[50];
} avisos;

typedef struct Editor {
    int nlinhas, ncolunas, l_atual, c_atual, n_palavras, n_letras;
} editor;

typedef struct Request {
    pid_t pid_cliente;
    int nr_linha;    
} request;

typedef struct Controlo {
    int perm;
    char np_name[10];
} controlo;



#endif