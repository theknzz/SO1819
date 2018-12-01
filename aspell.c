#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char correta[200], atual[200], lixo[200];
    int fd_ser, fd_cli, w, r;
    int ida[2];
    int volta[2];
    pid_t processo;

    if (pipe(ida) == -1)
    {
        fprintf(stderr, "\nFalha ao abrir o pipe ida!!!!!\n");
        return 0;
    }

    if (pipe(volta) == -1)
    {
        fprintf(stderr, "\nFalha ao abrir o pipe volta!!!!!\n");
        return 0;
    }

    processo = fork();

    while (strcmp(atual, "sair") != 0)
    {
        if (processo == 0)
        {
            // fazer do stdin a leitura do pipe ida
            if (dup2(ida[0], 0) < 0)
            {
                fprintf(stderr, "\nErro ao mudar os descritores stdin!!\n");
                exit(EXIT_FAILURE);
            }
            // fecha a leitura do pipe ida antiga
            close(ida[0]);
            // fazer do stdout a escrita no pipe volta
            if (dup2(volta[1], 1) < 0)
            {
                fprintf(stderr, "\nErro ao mudar os descritores stdout!!\n");
                exit(EXIT_FAILURE);
            }
            // fecha a escrita do pipe volta antiga
            close(volta[1]);
            // corre o aspell
            execl("aspell", "aspell", "-a", NULL);
        }
        if (processo < 0)
        {
            fprintf(stderr, "\nErro ao fazer o fork!!!\n");
        }
        if (processo > 0)
        {
            printf("\nEscreva uma frase para verificar:\n");
            scanf(" %[^\n]", atual);
            //lê o lixo do aspell
            read(volta[0], lixo, sizeof(lixo));
            //escreve na extremidade do pipe
            write(ida[1], atual, sizeof(atual));
            //lê o conteúdo do aspell
            read(volta[0], correta, sizeof(correta));

            printf("\nTotal: %s\n", atual);
            printf("\nCorreta: %s\n", correta);
        }
    }
    //fecha a escrita do pipe ida
    close(ida[1]);
    //fecha a leitura do pipe volta
    close(volta[0]);

    return 0;
}