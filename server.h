#ifndef __SERVER__
#define __SERVER__

#include "structs.h"

// mutex
pthread_mutex_t trinco;

// vars
int s_fifo_fd, pos, editores[MAXLINES];

// funcoes
void inicia_vars(editor *t, user *u, server *s);
int verifica_user( char *nome, container * box);
void getOption_ser(int argc, char **argv, editor *t, user *u, server *s);
void mostra_def(editor* t, server* s);
void *verificaCliente(void *dados);
void banner();
void dicionario(char *original, char *correta);
void termina1();
void cria_np_interacao();
void *employee(void *dados);
void requisita(int *editores, comunica *com);
void commandline(editor *edit, container *box);
void users_command(user *users);
int guarda_tabela(int linhas, int colunas, char *tab[linhas][colunas], char *nome_fich);
int carrega_tabela(int linhas, int colunas, char *tab[linhas][colunas], char *nome_fich);


#endif