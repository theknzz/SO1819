#ifndef __CLIENT__
#define __CLIENT__

#include "structs.h"
//#include <ncurses.h>
#include <curses.h>

void inicia_vars(editor *t, user *u, server *s);
void criar_editor(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas], char inter_fifo_fname[20]);
void getOption_cli(int argc, char **argv, user *u);

#endif