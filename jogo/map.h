#include "types.h"

#ifndef __MAP__
#define __MAP__

void drawMap(char mapa[MAP_SIZE][MAP_SIZE], struct pacman p);

void createMap(char mapa[MAP_SIZE][MAP_SIZE], char *filename);

#endif