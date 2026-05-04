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