#include "map.h"

#ifndef __PLAYER__
#define __PLAYER__

struct player
{
	unsigned int x;
	unsigned int y;
	unsigned int visao;
	unsigned int passos;
};

struct player spawnPlayer(char mapa[MAP_SIZE][MAP_SIZE]);
int movePlayer(struct player *p, char mapa[MAP_SIZE][MAP_SIZE], char direcao);
#endif