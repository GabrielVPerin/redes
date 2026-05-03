#include <stdlib.h>
#include "player.h"
#include "terminal.h"

struct player spawnPlayer(char mapa[MAP_SIZE][MAP_SIZE])
{
    unsigned int x, y;
    do
    {
        x = rand() % MAP_SIZE;
        y = rand() % MAP_SIZE;

    } while (mapa[y][x] != '0');

    struct player p;
    p.x = x;
    p.y = y;
    p.visao = 1;
    p.passos = 0;

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