#ifndef __SERVER__
#define __SERVER__

#include "structs.h"

// mutex
pthread_mutex_t trinco;

// vars
int s_fifo_fd, pos, editores[MAXLINES];

// funcoes
void inicia_vars(editor *t, user *u, server *s);
int verifica_user( char *nome, server *s);
void getOption_ser(int argc, char **argv, editor *t, user *u, server *s);
void mostra_def(editor* t, server* s);
void *verificaCliente(void *dados);
void serv_cli(server *s);
void commandline(editor* t, server* s);
void banner();
void dicionario(comunica *original);
void termina1();
void cria_np_interacao();
void *employee(void *dados);
void requisita(int *editores, comunica *com);


#endif