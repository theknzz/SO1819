#ifndef SERVER
#define SERVER
#include "structs.h"
// funcoes


void inicia_vars(editor *t, user *u, server *s);
void verifica_user( char *nome, char *file );
void getOption(int argc, char **argv, editor t, user u, server s);
void mostra_def(editor t, server s);

#endif