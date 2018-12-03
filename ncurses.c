#include "structs.h"
#include "client.h"

void criar_editor(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas], char inter_fifo_fname[20])
{
    WINDOW *erros;
    int tecla, i = 0, j, coluna_ini = 2, r;
    int tecla2;
    char x, aux[t->ncolunas];

    int c_fifo_fd, inter_fifo_fd;
    char c_fifo_fname[20];
    comunica com;

    memset(aux, ' ', t->ncolunas);

    // preenche o id do request
    com.request.pid_cliente = getpid();
    sprintf(c_fifo_fname, CLIENT_FIFO, com.request.pid_cliente);

    // cria o pipe
    if (mkfifo(c_fifo_fname, 0600) == -1)
    {
        perror("erro ao criar o np do cliente");
        exit(EXIT_FAILURE);
    }
    else
        printf("\npipe cliente criado\n");

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
    erros = newwin(15, 20, 2, t->ncolunas + 5);

    box(janela, '\0', ACS_HLINE); //Criação do border (WINDOW, tipo de border, tipo de border)

    keypad(janela, TRUE); // Permite ler as letras do teclado (FALSE por default)

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    wbkgd(janela, COLOR_PAIR(1));

    t->l_atual = 1;
    t->c_atual = 0;

    for (i = 0; i < t->nlinhas; i++)
    {
        mvwprintw(janela, t->l_atual, t->c_atual, "%2d", i);
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

    while (tecla != 27)
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
            // Verificar se o NP Servidor existe
            if (access(SERVER_FIFO_P, F_OK) != 0)
            {
                wprintw(erros, "Servidor não está iniciado...! Prima ESC para sair...");
                wrefresh(erros);
                wmove(janela, t->l_atual, t->c_atual);
                break;
            }

            //Permite aceder à tabela de editores
            com.request.aspell = 0;

            // abre o FIFO do cliente para escrita e leitura
            c_fifo_fd = open(c_fifo_fname, O_RDWR);
            if (c_fifo_fd == -1)
            {
                unlink(c_fifo_fname);
                printf("\nerro ao abrir o fifo do cliente");
            }

            // abre o FIFO do servidor para escrita
            inter_fifo_fd = open(inter_fifo_fname, O_WRONLY);
            if (inter_fifo_fd == -1)
            {
                printf("\nerro ao abrir o pipe de intecao\n");
                unlink(c_fifo_fname);
                exit(EXIT_FAILURE);
            }
            //linha selecionada e posta na struct request
            com.request.nr_linha = t->l_atual - 1;

            //envia a struct request para o server
            write(inter_fifo_fd, &com.request, sizeof(com.request));

            //le a resposta do servidor
            read(c_fifo_fd, &com.controlo, sizeof(com.controlo));

            if (com.controlo.perm == 1 && com.controlo.sair == 0)
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

                // Impede que salte o while
                tecla2 = 0;

                // Enquanto estiver no modo de edicao
                // e nao quiser sair ...

                while (tecla2 != 27 && tecla == 10 && com.controlo.sair == 0)
                {
                    getyx(janela, t->l_atual, t->c_atual);

                    if (t->c_atual >= t->ncolunas + 2)
                        wmove(janela, t->l_atual, t->c_atual - 1);
                    else if (t->c_atual < 2)
                        wmove(janela, t->l_atual, t->c_atual + 1);
                    else if (t->c_atual < 1)
                        wmove(janela, t->l_atual, t->c_atual + 2);

                    tecla2 = wgetch(janela);

                    // Verificar se o NP Servidor existe
                    if (access(SERVER_FIFO_P, F_OK) != 0)
                    {
                        attron(COLOR_PAIR(2));
                        mvwchgat(janela, t->l_atual, 0, 2, 0, 1, NULL);
                        attroff(COLOR_PAIR(2));
                        wprintw(erros, "Servidor não está iniciado...! Prima ESC para sair...");
                        wrefresh(erros);
                        wmove(janela, t->l_atual, t->c_atual);
                        break;
                    }

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
                        for (i = t->c_atual - 2; i < t->ncolunas; i++)
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
                        wmove(janela, t->l_atual, t->c_atual);
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
                        if (aux[t->ncolunas - 1] != ' ')
                        {
                            noecho();
                            break;
                        }

                        echo();
                        for (i = t->ncolunas - 1; i >= t->c_atual; i--)
                        {
                            aux[i] = aux[i - 1];
                        }

                        aux[t->c_atual - 2] = tecla2;

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
                        //Proibe aceder à tabela de editores e funciona o aspell
                        com.request.aspell = 1;

                        //Escreve a linha na estrutura
                        strcpy(com.request.texto, aux);

                        //Escreve a estrutura no pipe para o servidor
                        write(inter_fifo_fd, &com.request, sizeof(com.request));

                        //Lê a nova estrutura com a frase reformulada pelo aspell
                        r = read(c_fifo_fd, &com.request, sizeof(com.request));

                        j = 0;

                        //Testes para ver se a frase estava toda certa ou não
                        for (i = 0; i < strlen(com.request.texto); i++)
                        {
                            if (com.request.texto[i] == '*')
                                j++;
                            if (com.request.texto[i] == '&')
                            {
                                wprintw(erros, "A palavra %d na linha %d esta errada! ", j + 1, t->l_atual - 1);
                                tecla2 = 0;
                                j++;
                            }
                        }

                        //Se a frase estiver toda certa entra nesta condição
                        if (tecla2 == 10)
                        {
                            for (i = 0; i < t->ncolunas; i++)
                            {
                                tab[t->l_atual - 1][i] = aux[i];
                            }
                        }
                        break;
                    }
                }
                // Verificar se o NP Servidor existe
                if (access(SERVER_FIFO_P, F_OK) != 0)
                {
                    attron(COLOR_PAIR(2));
                    mvwchgat(janela, t->l_atual, 0, 2, 0, 1, NULL);
                    attroff(COLOR_PAIR(2));
                    wprintw(erros, "Servidor não está iniciado...! Prima ESC para sair...");
                    wrefresh(erros);
                    wmove(janela, t->l_atual, t->c_atual);
                    break;
                }
                //Permite aceder à tabela de editores
                com.request.aspell = 0;
                // envia a struct request para o server (para me retirar da tabela de editores)
                write(inter_fifo_fd, &com.request, sizeof(com.request));

            } // fim da permissao
            noecho();
            break;
        }
        /*if (com.controlo.sair == 1)
            break;*/
    }

    close(c_fifo_fd);
    close(inter_fifo_fd);
    unlink(c_fifo_fname);
    wrefresh(janela);
    endwin(); // Encerra o ncurses
}