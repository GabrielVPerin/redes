#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <arquivo.h>
<<<<<<< HEAD
=======
#include <protocolo.h>
#include <rede.h>

void recebe_visao(int soq, int lado)
{
    struct pacote pacote;

    int total = (lado * lado * 2) + 10;
    char *buffer = calloc(total, sizeof(char));

    int pos = 0;

    while (1)
    {
        rede_escuta(&pacote, soq);

        if (pacote.tipo == TIPO_FIM)
            break;

        if (pacote.tipo == TIPO_VISUALIZACAO)
        {
            int copy_len = pacote.tamanho;
            if (pos + copy_len > total)
                copy_len = total - pos;

            memcpy(buffer + pos, pacote.dados, copy_len);
            pos += copy_len;
        }
    }

    int col = 0;
    for (int i = 0; i < pos; i++)
    {
        char c = buffer[i];

        if (c == '\n')
        {
            if (col != 0) 
            {
                printf("\n");
                col = 0;
            }
            continue;
        }

        if (c == '\r' || c == '\0')
            continue;

        switch (c)
        {
        case 'X':
            printf("# ");
            break;

        case 'E':
        case '0':
        case 'V':
            printf("  ");
            break;

        default:
            // imprime P fantasmas arquivos
            printf("%c ", c);
            break;
        }

        col++;
        
        if (col == lado)
        {
            printf("\n");
            col = 0;
        }
    }

    if (col != 0)
        printf("\n");

    free(buffer);
}
>>>>>>> 3f5a81d (Envio e recepção de arquivos feito)

void desenharVisao()
{
    FILE *mapa = fopen("pacotaoDoPerin.csv", "r");
    if (!mapa)
    {
        fprintf(stderr, "Erro ao abrir o mapa no cliente\n");
        exit(1);
    }
    else
        fprintf(stderr, "Abriu mapa\n");
    char *linha = NULL;
    size_t capacidade = 0;

    while (getline(&linha, &capacidade, mapa) != -1)
    {
        linha[strcspn(linha, "\n")] = '\0';

        for (int j = 0; linha[j] != '\0'; j++)
        {
            switch (linha[j])
            {
            case 'X':
                printf("# ");
                break;
            case 'E':
            case '0':
                printf("  ");
                break;
            default:
                printf("%c ", linha[j]);
                break;
            }
        }

        printf("\n");
    }

    free(linha);
    fclose(mapa);
}

char movimento()
{
    return getchar();
}

struct termios oldt;

void reset_terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void init_terminal()
{
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
