#include <stdio.h>
#include <stdlib.h>
#include "entities.h"
#include "map.h"

void drawMap(char mapa[MAP_SIZE][MAP_SIZE], struct pacman p)
{

    printf("\n");
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (pacmanView(j, i, p))
            {

                switch (mapa[i][j])
                {
                case 'X':
                    printf("# ");
                    break;
                case 'E':
                case '0':
                    printf("  ");
                    break;
                default:
                    printf("%c ", mapa[i][j]);
                    break;
                }
            }
            else
                printf("  ");
        }
        printf("\n");
    }
}

// void drawMap(char mapa[MAP_SIZE][MAP_SIZE], struct pacman p)
// {
//     printf("\n");

// }

void createMap(char mapa[MAP_SIZE][MAP_SIZE], char *filename)
{

    FILE *file = fopen(filename, "r");
    if (!file)
        file = fopen("labirinto.csv", "r");
    if (!file)
        exit(1);

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            fscanf(file, " %c,", &mapa[i][j]);
        }
    }

    fclose(file);
}

char **drawPacmanView(char mapa[MAP_SIZE][MAP_SIZE], struct pacman p)
{
    int pV = (int)p.visao;

    char **mapView = malloc((pV*2 + 1) * sizeof(char *));
    for (size_t i = 0; i < (p.visao*2 + 1); i++)
        mapView[i] = malloc((pV*2 + 1) * sizeof(char));

    int xInicial = (int)p.x - pV;
    int yInicial = (int)p.y - pV;

    for (size_t i = 0; i < (p.visao*2 + 1); i++)
    {
        int y = yInicial + i;
        for (size_t j = 0; j < (p.visao*2 + 1); j++)
        {
            int x = xInicial + j;
            if (x < 0 || x >= 40 || y < 0 || y >= 40)
                mapView[i][j] = 'V';
            else
                mapView[i][j] = mapa[y][x];
        }
    }

    return mapView;
}