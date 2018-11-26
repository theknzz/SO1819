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

// defines
#define MAXLINES 15
#define MAXCOLUMNS 45
#define TIME_OUT 10
#define MAXUSERS 3

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

#endif