#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>

#include <protocolo.h>
#include <rede.h>
#include <arquivo.h>
#include "client.h"

int main()
{
    int soq = cria_raw_socket("lo");

    init_terminal();
    struct pacote pacote;

    char start = movimento();
    if (constroi_pacote(&pacote, sizeof(start), TIPO_INICIALIZACAO, (uint8_t *)&start))
        fprintf(stderr, "Erro ao construir pacote\n");
    printf("\nMandando Start\n");
    rede_envia(&pacote, soq);
    
    while (1)
    {

        arquivo_recebe(soq);
        fprintf(stderr, "\nDesenhando mapa\n");
        desenharVisao();

        do
        {
            fprintf(stderr, "\nEsperando movimento\n");
            char move = movimento();
            if (constroi_pacote(&pacote, sizeof(move), TIPO_DADOS, (uint8_t *)&move))
                fprintf(stderr, "Erro ao construir pacote\n");

            printf("\nMandando move\n");
            rede_envia(&pacote, soq);
            printf("\nEscutando resposta\n");
            rede_escuta(&pacote, soq);

            if (pacote.tipo == TIPO_ERRO)
            {
                fprintf(stderr, "\nMovimento invalido! Tente novamente.\n");
            }
            else if (pacote.tipo == TIPO_TXT || pacote.tipo == TIPO_JPG || pacote.tipo == TIPO_MP4)
            {
                desenharVisao();
                arquivo_recebe(soq);
            }
            else
                printf("\nMovimento Válido!\n");

        } while (pacote.tipo == TIPO_ERRO);
    }

    close(soq);

    reset_terminal();
    return EXIT_SUCCESS;
}
