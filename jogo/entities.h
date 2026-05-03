#include "types.h"

#ifndef __ENTITIES__
#define __ENTITIES__


struct player spawnPlayer(char mapa[MAP_SIZE][MAP_SIZE]);
int movePlayer(struct player *p, char mapa[MAP_SIZE][MAP_SIZE], char direcao);
int playerView(int x, int y, struct player p);


#endif