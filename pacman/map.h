#include "types.h"

#ifndef __MAP__
#define __MAP__

// Desenha no terminal o mapa (não está sendo usado)
// void drawMap(char mapa[MAP_SIZE][MAP_SIZE], struct pacman p);

// Gera o mapa de acordo com o .csv fornecido
void createMap(char mapa[MAP_SIZE][MAP_SIZE], char *filename);

// Retorna matriz com uma matriz da parte visível do mapa
char **drawPacmanView(char mapa[MAP_SIZE][MAP_SIZE], struct pacman p);

#endif