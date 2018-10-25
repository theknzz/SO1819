#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "structs.h"
#include "client.h"

void criar_editor(){
    int tecla;
    editor t;
    char tab[t.nlinhas][t.ncolunas];
    WINDOW *janela;
    initscr();
    start_color();
    clear();
    noecho();
    cbreak();

    janela = newwin(17,47, 2, 2);

    box(janela, ACS_VLINE, ACS_HLINE);

    scrollok(janela, TRUE);
    keypad(janela, TRUE);

    t.l_atual = 1;
    t.c_atual = 1;

    wmove(janela, t.l_atual, t.c_atual);
    
    wrefresh(janela);
    while(1){
        tecla = wgetch(janela);
        getyx(janela, t.l_atual, t.c_atual);
        switch(tecla){
        case KEY_UP:
            if(t.l_atual == 1)
                break;
            wmove(janela, t.l_atual - 1, t.c_atual);
            wrefresh(janela);
        break;
        case KEY_DOWN:
            if(t.l_atual == 15)
                break;
            wmove(janela, t.l_atual + 1, t.c_atual);
            wrefresh(janela);
        break;
        case KEY_RIGHT:
            if(t.c_atual == 45)
                break;
            wmove(janela, t.l_atual, t.c_atual + 1);
            wrefresh(janela);
        break;
        case KEY_LEFT:
            if(t.c_atual == 1)
                break;
            wmove(janela, t.l_atual, t.c_atual - 1);
            wrefresh(janela);
        break;
        }
        if(tecla == 27)
            break;
    }

    wrefresh(janela);
    endwin();
}

/*int main(int argc, char *argv) {

	char str[11], *pal[3];
	int n, i;

	fgets(str,12,stdin);
	puts(str);
	str[strlen(str)-1] = '\0';

	n = 0;
	pal[n] = strtok(str, " ");
	n++;
	while( (pal[n] = strtok( NULL, " ") ) != NULL)
		n++;

	for(i=0;i<n;i++)
		printf("%s\n",pal[i]);
	exit(0);
}*/

int main(){
	criar_editor();

    return 0;
}