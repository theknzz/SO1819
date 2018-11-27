#include "structs.h"
#include "client.h"
#include "server.h"

void criar_editor(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas])
{
    int tecla, i = 0, j, coluna_ini = 2;
    int tecla2;
    char x, aux[t->ncolunas];
    initscr();     // inicializa o uso do ncurses
    start_color(); // inicia as cores
    clear();       // limpa o ecrã
    noecho();      // não permite que se vejam teclas no ecrã
    cbreak();      // caso haja um ^C acaba o programa

    for (i = 0; i < t->nlinhas; i++)
    {
        for (j = 0; j < t->ncolunas; j++)
        {
            tab[i][j] = ' '; // inicialização da tabela como espaços
        }
    }

    janela = newwin(t->nlinhas + 2, t->ncolunas + 3, 2, 3); //Criação da janela (linhas, colunas, posiçãoy no stdscr, posiçãox no stdscr)

    box(janela, ' ', ACS_HLINE); //Criação do border (WINDOW, tipo de border, tipo de border)

    keypad(janela, TRUE); // Permite ler as letras do teclado (FALSE por default)

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    wbkgd(janela, COLOR_PAIR(1));

    t->l_atual = 1;
    t->c_atual = 2;

    for (i = 0; i < t->nlinhas; i++)
    {
        mvwprintw(janela, t->l_atual + i, t->c_atual - 2, "%2d", i); // Imprime o respetivo numero na linhas
        for (j = 0; j < t->ncolunas; j++)
        {
            mvwprintw(janela, t->l_atual, t->c_atual + j, " "); // Inicializa o ecrã com espaços
        }
    }

    wmove(janela, t->l_atual, t->c_atual); // Move o cursor da janela para uma posição da janela

    wrefresh(janela); // Atualiza a janela
    while (1)
    {
        tecla = wgetch(janela);                // Capta um caracter da janela e devolve o inteiro correspondente
        getyx(janela, t->l_atual, t->c_atual); // Atribui as coordenadas do cursor para as variáveis
        switch (tecla)
        {
        case KEY_UP:
            if (t->l_atual == 1) // Condições que não deixam o cursor tocar na box
                break;
            wmove(janela, t->l_atual - 1, t->c_atual); // Movimentações do cursor
            break;                                     //(Nunca esquecer de identificar a janela)
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
            attroff(COLOR_PAIR(2)); // Adiciona cor às linhas
            wmove(janela, t->l_atual, t->c_atual);
            wrefresh(janela);

            for (i = 0; i < t->ncolunas; i++)
            {
                aux[i] = tab[t->l_atual - 1][i];
            } // Cópia da tabela do editor para um vetor auxiliar
            
            tecla2 = 0; // Para impedir que salte o ciclo ao repetir

            while (tecla2 != 27)
            {
                getyx(janela, t->l_atual, t->c_atual);

                if (t->c_atual >= t->ncolunas + 2)
                    wmove(janela, t->l_atual, t->c_atual - 1);
                else if (t->c_atual < 2)
                    wmove(janela, t->l_atual, t->c_atual + 1);
                else if (t->c_atual < 1)
                    wmove(janela, t->l_atual, t->c_atual + 2);

                tecla2 = wgetch(janela);
                
                switch (tecla2)
                {
                case KEY_STAB:
                    break;
                case KEY_UP:
                    break;
                case KEY_DOWN:
                    break;
                case KEY_RIGHT:
                    if (t->c_atual == t->ncolunas + 1)
                        break;
                    wmove(janela, t->l_atual, t->c_atual + 1);
                    break;
                case KEY_LEFT:
                    if (t->c_atual == 2)
                        break;
                    wmove(janela, t->l_atual, t->c_atual - 1);
                    break;
                case KEY_BACKSPACE:
                    if (t->c_atual > 2)
                    {
                        for (i = t->c_atual - 2; i < t->ncolunas; i++)
                        {
                            aux[i - 1] = aux[i];
                        } //Puxa as teclas todas à direita para trás, o primeiro valor à esquerda é apagado
                        aux[t->ncolunas - 1] = ' '; // Acrescenta um espaço no fim da tabela auxiliar
                        for (i = 0; i < t->ncolunas; i++)
                        {
                            mvwprintw(janela, t->l_atual, i + 2, "%c", aux[i]);
                        }
                        wmove(janela, t->l_atual, t->c_atual - 1);// Move o cursor uma posição para trás
                    }
                    break;
                case KEY_DC:
                    for (i = t->c_atual - 2; i < t->ncolunas; i++)
                    {
                        aux[i] = aux[i + 1];
                    }//Puxa as teclas todas à direita para trás, o primeiro valor à direita é apagado
                    aux[t->ncolunas - 1] = ' ';// Acrescenta um espaço no fim da tabela auxiliar
                    for (i = 0; i < t->ncolunas; i++)
                    {
                        mvwprintw(janela, t->l_atual, i + 2, "%c", aux[i]);
                    }
                    wmove(janela, t->l_atual, t->c_atual);
                    break;

                case 27:
                    attron(COLOR_PAIR(2));
                    mvwchgat(janela, t->l_atual, 0, 2, 0, 1, NULL);
                    attroff(COLOR_PAIR(2)); // Retira a cor das linhas
                    wmove(janela, t->l_atual, 2);
                    for (i = 0; i < t->ncolunas; i++)
                    {
                        mvwprintw(janela, t->l_atual, i + 2, "%c", tab[t->l_atual - 1][i]);
                    }
                    wmove(janela, t->l_atual, t->c_atual);
                    break;

                case 10:
                    attron(COLOR_PAIR(2));
                    mvwchgat(janela, t->l_atual, 0, 2, 0, 1, NULL);
                    attroff(COLOR_PAIR(2));  // Retira a cor das linhas
                    wmove(janela, t->l_atual, t->c_atual);
                    break;

                default:
                    if(aux[t->ncolunas - 1] != ' '){
                        noecho();
                        break;
                    }// Verifica se a última posição da tabela está vazia para que não se escreva por cima

                    echo();
                    for (i = t->ncolunas - 1; i >= t->c_atual - 2; i--)
                    {
                        aux[i] = aux[i - 1];
                    }// Puxa as letras todas uma casa para a frente
                    
                    aux[t->c_atual - 2] = tecla2;// Atribui à casa atual a letra que recebeu
                    //t->n_palavras++;

                    for (i = 0; i < t->ncolunas; i++)
                    {
                        mvwprintw(janela, t->l_atual, i + 2, "%c", aux[i]);
                    }// Imprime a linha
                    
                    wmove(janela, t->l_atual, t->c_atual + 1);// Move o cursor uma posição para a frente
                    break;
                }

                if (tecla2 == 10)
                {
                    for (i = 0; i < t->ncolunas; i++)
                    {
                        tab[t->l_atual - 1][i] = aux[i];
                    }
                    break;
                } // Copia os valores da tabela auxiliar para a tabela do editor
            }
            noecho();
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

    inicia_vars(&t, &u, &s);

    char tab[t.nlinhas][t.ncolunas];

    criar_editor(janela, &t, tab);

    return 0;
}