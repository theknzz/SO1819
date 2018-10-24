//Código do cliente
#include "medit-defaults.h"
#include "client-defaults.h"

void criar_editor(){
    char tab[15][45];
    int tecla;
    int l_atual = 1, c_atual = 1;
    WINDOW *janela;
    initscr();
    start_color();
    clear();
    cbreak();

    janela = newwin(17,47, 2, 2);

    box(janela, ACS_VLINE, ACS_HLINE);

    scrollok(janela, TRUE);
    keypad(janela, TRUE);
    wmove(janela, l_atual, c_atual);
    
    wrefresh(janela);
    while(1){
        tecla = wgetch(janela);
        getyx(janela, l_atual, c_atual);
        switch(tecla){
        case KEY_UP:
            if(l_atual == 1)
                break;
            wmove(janela, l_atual - 1, c_atual);
            wrefresh(janela);
        break;
        case KEY_DOWN:
            wmove(janela, l_atual + 1, c_atual);
            wrefresh(janela);
        break;
        case KEY_RIGHT:
            wmove(janela, l_atual, c_atual + 1);
            wrefresh(janela);
        break;
        case KEY_LEFT:
            if(c_atual == 1)
                break;
            wmove(janela, l_atual, c_atual - 1);
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
