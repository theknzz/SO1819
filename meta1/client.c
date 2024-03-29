
#include "structs.h"
#include "client.h"
#include "server.h"

void guarda_info(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas])
{
    int coluna;

    for(coluna = 0, t->c_atual = 2; coluna < t->ncolunas && t->c_atual < t->ncolunas; coluna++, t->c_atual++)
    {
        mvwscanw(janela, t->l_atual, t->c_atual, "%c", &tab[t->l_atual - 1][coluna]);
        //mvwprintw(janela, t->l_atual + 1, 2, "%c", tab[t->l_atual][coluna]);
    }
}
void criar_editor(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas])
{
    int tecla = 0;
    char x;
    initscr(); // inicializa o uso do ncurses
    start_color(); // inicia as cores
    clear();   // limpa o ecrã
    noecho();   // não permite que se vejam teclas no ecrã
    cbreak();   // caso haja um ^C acaba o programa

    janela = newwin(t->nlinhas + 2, t->ncolunas + 3, 2, 3); //Criação da janela (linhas, colunas, posiçãoy no stdscr, posiçãox no stdscr)

    box(janela, ' ', ACS_HLINE);  //Criação do border (WINDOW, tipo de border, tipo de border)

    keypad(janela, TRUE);  // Permite ler as letras do teclado (FALSE por default)

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    wbkgd(janela, COLOR_PAIR(1));

    t->l_atual = 1;
    t->c_atual = 0;

    while(1){
        mvwprintw(janela, t->l_atual, t->c_atual, "%2d", (tecla + 1));
        if(t->l_atual == t->nlinhas)
            break;
        t->l_atual++;
        tecla++;
    }

    t->l_atual = 1;
    t->c_atual = 2;

    wmove(janela, t->l_atual, t->c_atual); // Move o cursor da janela para uma posição da janela

    wrefresh(janela);  // Atualiza a janela
    while (1)
    {
        tecla = wgetch(janela); // Capta um caracter da janela e devolve o inteiro correspondente
        getyx(janela, t->l_atual, t->c_atual);  // Atribui as coordenadas do cursor para as variáveis
        switch (tecla)
        {
        case KEY_UP:
            if (t->l_atual == 1)  // Condições que não deixam o cursor tocar na box
                break;
            wmove(janela, t->l_atual - 1, t->c_atual);  // Movimentações do cursor 
            break;                                    //(Nunca esquecer de identificar a janela)
        case KEY_DOWN:
            if (t->l_atual == 15)
                break;
            wmove(janela, t->l_atual + 1, t->c_atual);
            break;
        case KEY_RIGHT:
            if (t->c_atual == 46)
                break;
            wmove(janela, t->l_atual, t->c_atual + 1);
            break;
        case KEY_LEFT:
            if (t->c_atual == 2)
                break;
            wmove(janela, t->l_atual, t->c_atual - 1);
            break;
        case 10: // No caso de ENTER
            attron(COLOR_PAIR(2));
            mvwchgat(janela, t->l_atual, 0, 2, 0, 2, NULL);
            attroff(COLOR_PAIR(2));
            wmove(janela, t->l_atual, t->c_atual);
            wrefresh(janela);
            while (1)
            {
                getyx(janela, t->l_atual, t->c_atual);

				if(t->c_atual >= t->ncolunas + 2)
                    wmove(janela, t->l_atual, t->c_atual - 1);
                else if (t->c_atual < 2)
                    wmove(janela, t->l_atual, t->c_atual + 1);
                else if (t->c_atual < 1)
                    wmove(janela, t->l_atual, t->c_atual + 2);

                echo();
                
                tecla = wgetch(janela);

                switch (tecla)
                {
                    case KEY_RIGHT:
                        if (t->c_atual == t->ncolunas+1)
                            break;
                        wmove(janela, t->l_atual, t->c_atual + 1);
                        break;
                    case KEY_LEFT:
                        if (t->c_atual == 2)
                            break;
                        wmove(janela, t->l_atual, t->c_atual - 1);
                        break;
                    case KEY_BACKSPACE:
                        if(t->c_atual >= 3)
                            wdelch(janela);
                        break;
                    case KEY_DC:
                        wdelch(janela);
                        break;

                    case 27:
                        attron(COLOR_PAIR(2));
                        mvwchgat(janela, t->l_atual, 0, 2, 0, 1, NULL);
                        attroff(COLOR_PAIR(2));
                        wmove(janela, t->l_atual, 2);
                        wclrtoeol(janela);
                        break;
                    
                    case 10:
                        attron(COLOR_PAIR(2));
                        mvwchgat(janela, t->l_atual, 0, 2, 0, 1, NULL);
                        attroff(COLOR_PAIR(2));
                        wmove(janela, t->l_atual, 2);
                        break;

                    default:
                        winsch(janela, tecla);
                        break;
                }


                if (tecla == 27) //No caso de ESC
                {
                    break;
                }
                else 
                    //guarda_info(janela, t, tab);

                if(tecla == 10)
                {
                    break;
                }
            }
            noecho();
            tecla = 0; // Impedir saida do stdscr
            break;
        }
        if (tecla == 27)
            break;
    }

    wrefresh(janela);
    endwin(); // Encerra o ncurses
}

int main(int argc, char **argv)
{
    editor t;
    user u;
    server s;
    WINDOW *janela;

    inicia_vars(&t,&u,&s);
    
    char tab[t.nlinhas][t.ncolunas];

    criar_editor(janela, &t, tab);

    return 0;

}