#include "structs.h"
#include "client.h"
#include "server.h"

void criar_editor(WINDOW *janela, editor *t, char tab[t->nlinhas][t->ncolunas], char inter_fifo_fname[20], int c_fifo_fd)
{
    WINDOW *erros;
    int tecla, i = 0, j, coluna_ini = 2;
    int tecla2;
    char x, aux[t->ncolunas], res;

    int inter_fifo_fd;
    char c_fifo_fname[20];
    request req;

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

    erros = newwin(10, 30, 2, t->ncolunas + 6);

    box(janela, ' ', ACS_HLINE); //Criação do border (WINDOW, tipo de border, tipo de border)

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

    wprintw(erros, "pipe de interacao atribuido: %s", inter_fifo_fname);
    wrefresh(erros);

    wrefresh(janela); // Atualiza a janela
    while (1)
    {
        if (access(SERVER_FIFO_P, F_OK) != 0)
        {
            wprintw(erros, "\nServidor nao existe...!\n");
            wrefresh(erros);
            break;
        }
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
            inter_fifo_fd = open(inter_fifo_fname, O_WRONLY);
            if (inter_fifo_fd == -1)
            {
                unlink(c_fifo_fname);
                exit(EXIT_FAILURE);
            }

            // linha selecionada e posta na struct request
            req.nr_linha = t->l_atual - 1;
            req.pid_cliente = getpid();
            req.perm = 0;
            // envia a struct request para o server
            write(inter_fifo_fd, &req, sizeof(req));

            // le a resposta do servidor
            read(c_fifo_fd, &req, sizeof(req));

            if (req.perm == 1)
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
                    if (access(SERVER_FIFO_P, F_OK) != 0)
                    {
                        wprintw(erros, "\nServidor nao existe...!\n");
                        wrefresh(erros);
                        break;
                    }
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
                            if (aux[i] != ' ')
                                t->n_palavras++;
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
                strcpy(req.frase, aux);
                // envia a struct request para o server (para me retirar da tabela de editores)
                write(inter_fifo_fd, &req, sizeof(req));
                close(inter_fifo_fd);
            } // fim da permissao

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

    char val_fifo_fname[20], inter_fifo_fname[20], c_fifo_fname[20];
    valida val;
    int c_fifo_fd, res, s_fifo_fd, r, w, inter_fifo_fd;

    // Verificar se o NP Servidor existe
    if (access(SERVER_FIFO_P, F_OK) != 0)
    {
        fprintf(stderr, "\nServidor não está iniciado...!\n");
        exit(EXIT_FAILURE);
    }

    // abre o np do servidor
    s_fifo_fd = open(SERVER_FIFO_P, O_WRONLY);
    if (s_fifo_fd == -1)
    {
        fprintf(stderr, "\nO pipe principal do server não abriu!\n");
        exit(EXIT_FAILURE);
    }

    // preenche o id para validacao
    val.pid_user = getpid();
    sprintf(c_fifo_fname, CLIENT_FIFO, val.pid_user);

    // cria o pipe
    res = mkfifo(c_fifo_fname, 0600);
    if (res == -1)
    {
        printf("\nErro ao criar o pipe do cliente!\n");
        exit(EXIT_FAILURE);
    }

    getOption_cli(argc, argv, &u);

    if (strlen(u.nome) == 0)
    {
        // preenche o nome do user
        printf("\nInsira o nome de utilizador: ");
        scanf(" %7[^\n]", val.nome);
    }
    else
        strcpy(val.nome, u.nome);

    // abre o FIFO do cliente para escrita e leitura
    c_fifo_fd = open(c_fifo_fname, O_RDWR);
    if (c_fifo_fd == -1)
    {
        unlink(c_fifo_fname);
        exit(EXIT_FAILURE);
    }

    // manda a informacao ao servidor
    // para ser validada
    w = write(s_fifo_fd, &val, sizeof(val));
    if (w == sizeof(val))
        fprintf(stderr, "\nValidacao bem enviada...\n", w);

    // le a informação recebida do servidor
    // validation info
    r = read(c_fifo_fd, &val, sizeof(val));
    if (r == sizeof(val))
        fprintf(stderr, "\nValidacao bem lida...\n");

    // USER PASSA A SER UM CLIENTE

    // se o utiizador for cliente
    if (val.ver == 1)
    {
        strcpy(inter_fifo_fname, val.np_name);
        /*inter_fifo_fd = open(inter_fifo_fname, O_WRONLY);

        if (inter_fifo_fd == -1)
        {
            fprintf(stderr, "\n O pipe val nao abriu\n");
            close(s_fifo_fd);
            exit(EXIT_FAILURE);
        }*/

        inicia_vars(&t, &u, &s);
        char tab[t.nlinhas][t.ncolunas];
        strcpy(u.nome, val.nome);
        /*printf("\n%s\n", val.np_name);
        printf("\n%d\n", val.pid_user);
        sleep(3);*/
        criar_editor(janela, &t, tab, inter_fifo_fname, c_fifo_fd);
        close(c_fifo_fd);
        close(inter_fifo_fd);
        unlink(c_fifo_fname);
    }
    else if (val.ver == 0)
        fprintf(stderr, "\n '%s' nao consta na base de dados\n", val.nome);

    return 0;
}