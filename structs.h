#ifndef STRUCTS
#define STRUCTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int n_utilizadores, n_palavras, n_letras, fich_utilizadores;
    char fich_nome[10], chars_mais_comuns[5];
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

#endif