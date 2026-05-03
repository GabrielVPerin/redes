#include <stdlib.h>
#include "entities.h"
#include "terminal.h"

void randSpawn(unsigned int *x, unsigned int *y)
{
    x = rand() % MAP_SIZE;
    y = rand() % MAP_SIZE;
}

struct player spawnPlayer(char mapa[MAP_SIZE][MAP_SIZE])
{
    struct player p;
    p.visao = 1;
    p.passos = 0;

    // Caso o mapa venha com o player definido
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            if (mapa[i][j] == 'P')
            {
                p.x = j;
                p.y = i;
                return p;
            }
        }
    }

    unsigned int x, y;
    do
    {
        randSpawn(&x,&y);

    } while (mapa[y][x] != '0');

    p.x = x;
    p.y = y;

    mapa[y][x] = 'P';

    return p;
}

int movePlayer(struct player *p, char mapa[MAP_SIZE][MAP_SIZE], char direcao)
{
    if (mapa[p->y][p->x] != 'P')
    {
        reset_terminal();
        exit(1);
    }

    struct player aux = *p;

    switch (direcao)
    {
    case 'w':
        aux.y--;
        break;
    case 'a':
        aux.x--;
        break;
    case 's':
        aux.y++;
        break;
    case 'd':
        aux.x++;
        break;
    default:
        break;
    }

    if (mapa[aux.y][aux.x] != '0')
        return 1;
    mapa[p->y][p->x] = '0';
    *p = aux;
    mapa[p->y][p->x] = 'P';
    p->passos++;
    if (p->passos % 5 == 0)
        p->visao++;

    return 0;
}

int playerView(int x, int y, struct player p)
{
    int dx = x - (int)p.x;
    int dy = y - (int)p.y;
    int r = (int)p.visao;

    return (dx * dx + dy * dy) <= (r * r + r);
}