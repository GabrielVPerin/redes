#include <types.h>
#include <protocolo.h>

#ifndef __ENTITIES__
#define __ENTITIES__

struct entities spawnEntities(char mapa[MAP_SIZE][MAP_SIZE]);

// Move o player e detecta colisão com fantasmas e arquivos
int movePacman(struct pacman *p, char mapa[MAP_SIZE][MAP_SIZE], char direcao, int soq, struct pacote *pacote);

// Retorna v/f se a coordenada passa está no range de visão do player
int pacmanView(int x, int y, struct pacman p);

void moveAllGhosts(char mapa[MAP_SIZE][MAP_SIZE], struct entities *entities);

#endif