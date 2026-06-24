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
    int soq = cria_raw_socket("lo");
    struct pacote pacote;
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

    while (1)
    {
        fprintf(stderr, "\n\n\n");

        // envia mapa visível ao cliente
        envia_mapa_visivel(mapa, pacMan, &pacote, soq);

        int movePossivel;
        do
        {
            fprintf(stderr, "Esperando movimento\n");
            rede_escuta(&pacote, soq); // recebe movimento do cliente
            move = pacote.tipo;
            fprintf(stderr, "\nVivos: %d\n", qtdArquivosVivos);
            
            if (qtdArquivosVivos == 0)
            {
                zerou_jogo(&pacote, soq);
                return 0;
            }

            movePossivel = movePacman(&pacMan, mapa, move, soq, &pacote);
            feedback_movimento(movePossivel, &pacote, soq);

        } while (movePossivel != 0); // 0 é quando o movimento é legal

        moveAllGhosts(mapa, &entities);
    }
    return EXIT_SUCCESS;
}