#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <arquivo.h>

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
