#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <protocolo.h>
#include <rede.h>
#include <arquivo.h>
#include <entities.h>
#include <map.h>
#include "auxiliares.h"

int qtdArquivosVivos = 6;

int main()
{
    int soq = cria_raw_socket("enp3s0");
    struct pacote pacote;
    fprintf(stderr, "\nEsperando start do cliente...");
    rede_escuta(&pacote, soq);

    if (pacote.tipo != TIPO_INICIALIZACAO)
    {
        fprintf(stderr, "\nComunicação não iniciada\n");
        exit(1);
    }

    srand(time(NULL));
    char mapa[MAP_SIZE][MAP_SIZE];

    recebe_mapa(mapa, &pacote, soq);

    struct entities entities = spawnEntities(mapa);
    struct pacman pacMan = entities.pacman;
    char move;
    envia_mapa_visivel(mapa, pacMan, &pacote, soq);

    while (1)
    {

        int movePossivel;
        do
        {
            rede_escuta(&pacote, soq); // recebe movimento do cliente
            move = pacote.tipo;

            if (qtdArquivosVivos == 0)
            {
                zerou_jogo(&pacote, soq);
                return 0;
            }

            movePossivel = movePacman(&pacMan, mapa, move, soq, &pacote);
            feedback_movimento(movePossivel, &pacote, soq);

        } while (movePossivel != 0); // 0 é quando o movimento é legal

        moveAllGhosts(mapa, &entities);

        // envia mapa visível ao cliente
        envia_mapa_visivel(mapa, pacMan, &pacote, soq);
    }
    return EXIT_SUCCESS;
}