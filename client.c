#include "structs.h"
#include "client.h"
#include "server.h"

void criar_editor()
{
    int tecla = 0;
    editor t;
    WINDOW *janela;  // ponteiro para uma janela(como se fosse para um ficheiro)
    
    initscr(); // inicializa o uso do ncurses
    start_color(); // inicia as cores
    clear();   // limpa o ecrã
    noecho();   // não permite que se vejam teclas no ecrã
    cbreak();   // caso haja um ^C acaba o programa

    janela = newwin(17, 48, 2, 3); //Criação da janela (linhas, colunas, posiçãoy no stdscr, posiçãox no stdscr)

    box(janela, ' ', ACS_HLINE);  //Criação do border (WINDOW, tipo de border, tipo de border)

    keypad(janela, TRUE);  // Permite ler as letras do teclado (FALSE por default)

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    wbkgd(janela, COLOR_PAIR(1));

    t.l_atual = 1;
    t.c_atual = 0;

    while(1){
        mvwprintw(janela, t.l_atual, t.c_atual, "%2d", (tecla + 1));
        if(t.l_atual == 15)
            break;
        t.l_atual++;
        tecla++;
    }

    t.l_atual = 1;
    t.c_atual = 2;

    wmove(janela, t.l_atual, t.c_atual); // Move o cursor da janela para uma posição da janela

    wrefresh(janela);  // Atualiza a janela
    while (1)
    {
        tecla = wgetch(janela); // Capta um caracter da janela e devolve o inteiro correspondente
        getyx(janela, t.l_atual, t.c_atual);  // Atribui as coordenadas do cursor para as variáveis
        switch (tecla)
        {
        case KEY_UP:
            if (t.l_atual == 1)  // Condições que não deixam o cursor tocar na box
                break;
            wmove(janela, t.l_atual - 1, t.c_atual);  // Movimentações do cursor 
            break;                                    //(Nunca esquecer de identificar a janela)
        case KEY_DOWN:
            if (t.l_atual == 15)
                break;
            wmove(janela, t.l_atual + 1, t.c_atual);
            break;
        case KEY_RIGHT:
            if (t.c_atual == 46)
                break;
            wmove(janela, t.l_atual, t.c_atual + 1);
            break;
        case KEY_LEFT:
            if (t.c_atual == 2)
                break;
            wmove(janela, t.l_atual, t.c_atual - 1);
            break;
        case 10: // No caso de ENTER
            attron(COLOR_PAIR(2));
            mvwchgat(janela, t.l_atual, 0, 2, 0, 2, NULL);
            attroff(COLOR_PAIR(2));
            wmove(janela, t.l_atual, t.c_atual);
            wrefresh(janela);
            while (1)
            {
                getyx(janela, t.l_atual, t.c_atual);

				if(t.c_atual >= 46)
                    wmove(janela, t.l_atual, t.c_atual - 1);
                else if (t.c_atual < 2)
                    wmove(janela, t.l_atual, t.c_atual + 1);
                else if (t.c_atual < 1)
                    wmove(janela, t.l_atual, t.c_atual +2);

                echo();
                idlok(janela, TRUE);
                tecla = wgetch(janela);

                switch (tecla)
                {
                    case KEY_RIGHT:
                        if (t.c_atual == 46)
                            break;
                        wmove(janela, t.l_atual, t.c_atual + 1);
                        break;
                    case KEY_LEFT:
                        if (t.c_atual == 2)
                            break;
                        wmove(janela, t.l_atual, t.c_atual - 1);
                        break;
                    case KEY_BACKSPACE:
                        if(t.c_atual > 2)
                            wdelch(janela);
                        break;
                    case KEY_DC:
                        wdelch(janela);
                }
                if (tecla == 27) //No caso de ESC
                {
                    attron(COLOR_PAIR(2));
                    mvwchgat(janela, t.l_atual, 0, 2, 0, 1, NULL);
                    attroff(COLOR_PAIR(2));
                    wmove(janela, t.l_atual, 2);
                    wclrtoeol(janela);
                    break;
                }

                if(tecla == 10)
                {
                    attron(COLOR_PAIR(2));
                    mvwchgat(janela, t.l_atual, 0, 2, 0, 1, NULL);
                    attroff(COLOR_PAIR(2));
                    wmove(janela, t.l_atual, 2);
                    break;
                }
            }
            noecho();
            tecla = wgetch(janela);
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
    inicia_vars(&t,&u,&s);
    criar_editor();

    return 0;
}