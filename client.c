#include "structs.h"
#include "client.h"
#include "server.h"

/*void guarda_info(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas])
{
    int coluna;

    for(coluna = 0, t->c_atual = 2; coluna < t->ncolunas && t->c_atual < t->ncolunas; coluna++, t->c_atual++)
    {
        mvwscanw(janela, t->l_atual, t->c_atual, "%c", &tab[t->l_atual - 1][coluna]);
        //mvwprintw(janela, t->l_atual + 1, 2, "%c", tab[t->l_atual][coluna]);
    }
}*/

void criar_editor(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas])
{
    int tecla, i = 0, j, coluna_ini = 2;
    int tecla2;
    char x, aux[t->ncolunas];


    int s_fifo_fd, c_fifo_fd;
    request req;
    controlo ctrl;
    char c_fifo_fname[20];

        // preenche o id do request
        req.pid_cliente = getpid();
        sprintf(c_fifo_fname, CLIENT_FIFO, req.pid_cliente);

        // cria o pipe
        if(mkfifo(c_fifo_fname, 0600) == -1) {
            //perror("\nmkfifo FIFO do cliente deu erro");
            exit(EXIT_FAILURE);
        }
        //fprintf(stderr, "\nFIFO do cliente criado");


    initscr();     // inicializa o uso do ncurses
    start_color(); // inicia as cores
    clear();       // limpa o ecrã
    noecho();      // não permite que se vejam teclas no ecrã
    cbreak();      // caso haja um ^C acaba o programa

    for (i = 0; i < t->nlinhas; i++)
    {
        for (j = 0; j < t->ncolunas; j++)
        {
            tab[i][j] = ' '; // inicialização da tabela
        }
    }

    janela = newwin(t->nlinhas + 2, t->ncolunas + 3, 2, 3); //Criação da janela (linhas, colunas, posiçãoy no stdscr, posiçãox no stdscr)

    box(janela, ' ', ACS_HLINE); //Criação do border (WINDOW, tipo de border, tipo de border)

    keypad(janela, TRUE); // Permite ler as letras do teclado (FALSE por default)

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    wbkgd(janela, COLOR_PAIR(1));

    t->l_atual = 1;
    t->c_atual = 0;

    for (i = 0; i < t->nlinhas; i++)
    {
        mvwprintw(janela, t->l_atual, t->c_atual, "%2d", (i + 1 ));
        if (t->l_atual == t->nlinhas)
            break;
        t->l_atual++;
    }

    t->l_atual = 1;

    for (i = 0; i < t->nlinhas; i++)
    {
        t->c_atual = 2;
        for (j = 0; j < t->ncolunas; j++)
        {
            mvwprintw(janela, t->l_atual, t->c_atual, " ");
            t->c_atual++;
        }
        t->l_atual++;
    } //inicializar o ambiente do editor

    t->l_atual = 1;
    t->c_atual = 2;

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
            
            // abre o FIFO do servidor para escrita
            s_fifo_fd = open(SERVER_FIFO_P, O_WRONLY);
            if(s_fifo_fd == -1) {
                //fprintf(stderr, "\nO servidor nao esta a correr\n");
                unlink(c_fifo_fname);
                exit(EXIT_FAILURE);
            }
            //fprintf(stderr, "\nFIFO do servidor aberto WRITE / BLOCKING");

            // abre o FIFO do cliente para escrita e leitura
            c_fifo_fd = open(c_fifo_fname, O_RDWR);
            if(c_fifo_fd == -1) {
                //perror("\nErro ao abrir o FIFO do cliente");
                close(s_fifo_fd);
                unlink(c_fifo_fname);
                exit(EXIT_FAILURE);
            }
            //fprintf(stderr, "\nFIFO do cliente aberto para READ (+WRITE)/ bloqueante");

            // linha selecionada e posta na struct request
            req.nr_linha = t->l_atual;
            
            // envia a struct request para o server        
            write(s_fifo_fd, &req, sizeof(req));
            //printf("\nEnviei %d bytes ... linha: %d", w, req.nr_linha);
            // le a resposta do servidor
            read(c_fifo_fd, &ctrl, sizeof(ctrl));
            // if ( r == sizeof(ctrl))
            //     printf("\nPermissao: %d \nFlag: %s", ctrl.perm, ctrl.np_name);
            // else
            //     printf("\nSem resposta ou resposta incompreensivel" 
            //            "\nRecebi %d bytes", r);

            if(ctrl.perm == 1)
            {
                    attron(COLOR_PAIR(2));
                    mvwchgat(janela, t->l_atual, 0, 2, 0, 2, NULL);
                    attroff(COLOR_PAIR(2));
                    wmove(janela, t->l_atual, t->c_atual);
                    wrefresh(janela);

                    // copiar a linha existente para uma linha aux
                    for (i = 0; i < t->ncolunas; i++)
                    {
                        aux[i] = tab[t->l_atual - 1][i];
                    }
                    // Enquanto estiver no modo de edicao
                    // e nao quiser sair ...
                    while (tecla2 != 27 && tecla == 10)
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
                                }
                                aux[t->ncolunas - 1] = ' ';
                                for (i = 0; i < t->ncolunas; i++)
                                {
                                    mvwprintw(janela, t->l_atual, i + 2, "%c", aux[i]);
                                }
                                wmove(janela, t->l_atual, t->c_atual - 1);
                            }
                        break;
                        
                        case KEY_DC:
                            for (i = t->c_atual - 1; i < t->ncolunas; i++)
                            {
                                aux[i] = aux[i + 1];
                            }
                            aux[t->ncolunas - 1] = ' ';
                            for (i = 0; i < t->ncolunas; i++)
                            {
                                mvwprintw(janela, t->l_atual, i + 2, "%c", aux[i]);
                            }
                            wmove(janela, t->l_atual, t->c_atual);
                        break;

                        case 27:
                            attron(COLOR_PAIR(2));
                            mvwchgat(janela, t->l_atual, 0, 2, 0, 1, NULL);
                            attroff(COLOR_PAIR(2));
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
                            attroff(COLOR_PAIR(2));
                            wmove(janela, t->l_atual, t->c_atual);
                        break;

                        default:
                            if(aux[t->ncolunas - 1] != ' '){
                                noecho();
                                break;
                            }

                            echo();
                            for (i = t->ncolunas - 1; i >= t->c_atual; i--)
                            {
                                aux[i] = aux[i - 1];
                            }
                            aux[t->c_atual - 2] = tecla2;
                            //t->n_palavras++;

                            for (i = 0; i < t->ncolunas; i++)
                            {
                                mvwprintw(janela, t->l_atual, i + 2, "%c", aux[i]);
                            }
                            
                            t->c_atual++;
                            wmove(janela, t->l_atual, t->c_atual);
                            break;
                        }

                        if (tecla2 == 10)
                        {
                            for (i = 0; i < t->ncolunas; i++)
                            {
                                tab[t->l_atual - 1][i] = aux[i];
                            }
                            break;
                        }
                    }
                    // envia a struct request para o server (para me retirar da tabela de editores)      
                    write(s_fifo_fd, &req, sizeof(req));

            } // fim da permissao

            noecho();
            break;
        }
        if (tecla == 27)
            break;
    }
    close(c_fifo_fd);
    close(s_fifo_fd);
    unlink(c_fifo_fname);
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