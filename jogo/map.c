#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "map.h"

void drawMap(char mapa[MAP_SIZE][MAP_SIZE])
{

    printf("\n");
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            switch (mapa[i][j])
            {
            case 'X':
                printf("# ");
                break;
            case 'P':
                printf("P ");
                break;
            default:
                printf("  ");
                break;
            }
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