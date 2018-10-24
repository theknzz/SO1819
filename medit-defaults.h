#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>



typedef struct {
    int n_utilizadores, n_palavras, n_letras;
    char fich_utilizadores[15];
} server;

typedef struct {
    char nome[8];
    int tempo_linha, linhas_editadas, linha_atual;
} user;

typedef struct {
    int tipo;
    char aviso[50];
} avisos;

typedef struct {
    int nlinhas, ncolunas;
    int posx, posy;
} editor;

typedef struct {
    char nome[10];
} fich;