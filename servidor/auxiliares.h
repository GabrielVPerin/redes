#ifndef __AUXILIARES__
#define __AUXILIARES__

#include <stdio.h>
#include <stdlib.h>
#include <protocolo.h>
#include <types.h>


// recebe mapa (ou não) do cliente
void recebe_mapa(char mapa[MAP_SIZE][MAP_SIZE], struct pacote *pacote, int soq);

void envia_mapa_visivel(char mapa[MAP_SIZE][MAP_SIZE], struct pacman pacMan, struct pacote *pacote, int soq);

// chamada quando acaba os arquivos do mapa
void zerou_jogo(struct pacote *pacote, int soq);

// retorna pro cliente se o movimento foi válido ou não
void feedback_movimento(int moveP, struct pacote *pacote, int soq);

#endif