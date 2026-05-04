#include "types.h"

#ifndef __ENTITIES__
#define __ENTITIES__


struct entities spawnEntities(char mapa[MAP_SIZE][MAP_SIZE]);
int movePacman(struct pacman *p, char mapa[MAP_SIZE][MAP_SIZE], char direcao);
int pacmanView(int x, int y, struct pacman p);
void moveAllGhosts(char mapa[MAP_SIZE][MAP_SIZE], struct entities *entities);

#endif