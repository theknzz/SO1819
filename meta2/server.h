#ifndef __SERVER__
#define __SERVER__

#include "structs.h"

// vars
int inter_fifo_fd, s_fifo_fd, pos, SAIR;

// funcoes
void inicia_vars(editor *t, user *u, server *s);
int verifica_user( char *nome, server *s);
void getOption_ser(int argc, char **argv, editor *t, user *u, server *s);
void mostra_def(editor t, server s);
void *verificaCliente(void *dados);
void *serv_cli(void *dados);
void commandline(editor* t, server* s);
void banner();
void dicionario(comunica *original);
void termina1();

#endif