#ifndef __CLIENT__
#define __CLIENT__

#include "structs.h"
//#include <ncurses.h>

WINDOW *janela;

void inicia_vars(editor *t, user *u, server *s);
void criar_editor(editor *t, char tab[t->nlinhas][t->ncolunas], char inter_fifo_fname[20]);
void getOption_cli(int argc, char **argv, user *u);
void avisa_cli();

#endif