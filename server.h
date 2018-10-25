#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#ifndef SERVER
#define SERVER

// funcoes

void inicia_vars();
void verifica_user( char *nome, char *file );
void menu();
void getOption(int argc, char **argv);


#endif