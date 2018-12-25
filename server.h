#ifndef __SERVER__
#define __SERVER__

#include "structs.h"

// mutex
pthread_mutex_t *trinco;

// vars
int s_fifo_fd, pos, editores[MAXLINES], nr_np, nr_max_users;
user *users;
time_t start_t, end_t;

// funcoes
void inicia_vars(editor *t, user *u, server *s);
int verifica_user(char *nome, server *s);
void getOption_ser(int argc, char **argv, editor *t, user *u, server *s);
void mostra_def(editor* t, server* s);
void *verificaCliente(void *dados);
void banner();
void dicionario(char *original, char *correta);
void termina1();
void cria_np_interacao();
void *employee(void *dados);
void requisita(int *editores, comunica *com);
void commandline(editor *edit, server *s, editor *t);
void users_command(user *users);
int guarda_tabela(int linhas, int colunas, char *tab[linhas][colunas], char *nome_fich);
int carrega_tabela(int linhas, int colunas, char *tab[linhas][colunas], char *nome_fich);
void text_command(char *tab[], editor *t);
void free_command(char **tab, editor *t, int num);

#endif