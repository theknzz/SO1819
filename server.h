#ifndef SERVER
#define SERVER
#include "structs.h"


// funcoes

void inicia_vars(editor *t, user *u, server *s);
int verifica_user( char *nome, server *s);
void getOption(int argc, char **argv, editor *t, user *u, server *s);
void mostra_def(editor t, server s);

#endif