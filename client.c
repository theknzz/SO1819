#include "structs.h"
#include "client.h"
#include "server.h"

void criar_editor()
{
    int tecla;
    editor t;
    char tab[t.nlinhas][t.ncolunas];
    WINDOW *janela;  // ponteiro para uma janela(como se fosse para um ficheiro)
    initscr(); // inicializa o uso do ncurses
    start_color(); // inicia as cores
    clear();   // limpa o ecrã
    noecho();   // não permite que se vejam teclas no ecrã
    cbreak();   // caso haja um ^C acaba o programa

    janela = newwin(17, 47, 2, 2); //Criação da janela (linhas, colunas, posiçãoy no stdscr, posiçãox no stdscr)

    box(janela, ACS_VLINE, ACS_HLINE);  //Criação do border (WINDOW, tipo de border, tipo de border)

    scrollok(janela, TRUE); // Não permite as cenas passarem para fora da janela (WINDOW, TRUE ou FALSE)
    keypad(janela, TRUE);  // Permite ler as letras do teclado (FALSE por default)

    t.l_atual = 1;
    t.c_atual = 1;

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
            if (t.c_atual == 45)
                break;
            wmove(janela, t.l_atual, t.c_atual + 1);
            break;
        case KEY_LEFT:
            if (t.c_atual == 1)
                break;
            wmove(janela, t.l_atual, t.c_atual - 1);
            break;
        case 10: // No caso de ENTER
            echo(); // Mostra as letras
            wrefresh(janela);
            while (1)
            {
                tecla = wgetch(janela);
                getyx(janela, t.l_atual, t.c_atual);
                switch (tecla)
                {
                case KEY_RIGHT:
                    if (t.c_atual == 45)
                        break;
                    wmove(janela, t.l_atual, t.c_atual + 1);
                    break;
                case KEY_LEFT:
                    if (t.c_atual == 1)
                        break;
                    wmove(janela, t.l_atual, t.c_atual - 1);
                    break;

                }
                if (tecla == 27) //No caso de ESC
                    {
                        //descarta_info();
                        noecho();
                        break;
                    }
            }
            tecla = 0;
            break;
        }
        if (tecla == 27)
            break;
    }

    wrefresh(janela);
    endwin(); // Encerra o ncurses
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

int main(int argc, char **argv)
{
    //getUser(argc, argv);
    criar_editor();

    return 0;
}