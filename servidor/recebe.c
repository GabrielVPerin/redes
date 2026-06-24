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

int main(int argc, char *argv[])
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

    if (argc > 1)
        createMap(mapa, argv[argc - 1]);
    else
        createMap(mapa, NULL);

    struct entities entities = spawnEntities(mapa);
    struct pacman pacMan = entities.pacman;

    char move;

    while (1)
    {
        char **mapView = drawPacmanView(mapa, pacMan);

        for (int i = 0; i < ((int)pacMan.visao * 2 + 1); i++)
        {
            for (int j = 0; j < ((int)pacMan.visao * 2 + 1); j++)
            {
                switch (mapView[i][j])
                {
                case 'X':
                    printf("# ");
                    break;
                case 'V':
                    break;
                case 'E':
                case '0':
                    printf("  ");
                    break;
                default:
                    printf("%c ", mapView[i][j]);
                    break;
                }
            }
            printf("\n");
        }

        FILE *enviarCliente = fopen("pacotaoDoPerin.csv", "w");
        if (!enviarCliente)
        {
            fprintf(stderr, "Erro ao abrir arquivo\n");
            return 1;
        }

        for (int i = 0; i < (int)pacMan.visao * 2 + 1; i++)
        {
            int flag = 0;
            for (int j = 0; j < (int)pacMan.visao * 2 + 1; j++)
            {
                if (mapView[i][j] != 'V')
                {
                    fwrite(&mapView[i][j], sizeof(char), 1, enviarCliente);
                    flag = 1;
                }
            }
            if (flag)
                fputc('\n', enviarCliente);
        }

        fclose(enviarCliente);

        fprintf(stderr, "Enviando mapa.csv\n");
        envia_csv("pacotaoDoPerin.csv", soq);

        for (int i = 0; i < ((int)pacMan.visao * 2 + 1); i++)
            free(mapView[i]);
        free(mapView);

        int moveP;
        do
        {
            fprintf(stderr, "Esperando movimento\n");
            rede_escuta(&pacote, soq); // recebe movimento do cliente
            move = (char)pacote.dados[0];
            moveP = movePacman(&pacMan, mapa, move, soq, &pacote);

            if (moveP == -1)
            {
                fprintf(stderr, "Movimento Invalido\n");
                if (constroi_pacote(&pacote, sizeof(moveP), TIPO_ERRO, (uint8_t *)&moveP))
                    fprintf(stderr, "Erro ao construir pacote");
                fprintf(stderr, "Mandando Erro sobre o movimento\n");
                rede_envia(&pacote, soq);
            }
            else // MOVIMENTO VALIDO
            {
                fprintf(stderr, "Movimento Valido\n");
                if (constroi_pacote(&pacote, sizeof(moveP), TIPO_DADOS, (uint8_t *)&moveP))
                    fprintf(stderr, "Erro ao construir pacote");
                fprintf(stderr, "Mandando movimento correto\n");
                rede_envia(&pacote, soq);
            }

        } while (moveP != 0); // 0 é quando o movimento é legal

        moveAllGhosts(mapa, &entities);
    }
    return EXIT_SUCCESS;
}