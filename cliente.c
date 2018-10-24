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

int main(){
	criar_editor();

    return 0;
}
