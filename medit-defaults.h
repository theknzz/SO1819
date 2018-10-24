#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int n_utilizadores, n_palavras, n_letras;
    char fich_utilizadores[15], chars_mais_comuns[5];
} servidor;

typedef struct {
    char nome[50];
    int tempo_linha, linhas_editadas, linha_atual;
} utilizador;

typedef struct {
    int tipo;
    char aviso[50];
} avisos;

typedef struct {
    int nlinhas, ncolunas;
    int posx, posy;
} editor;