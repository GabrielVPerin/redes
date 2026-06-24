#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <protocolo.h>
#include <rede.h>
#include <arquivo.h>
#include "client.h"

int main(int argc, char *argv[])
{
    int soq = cria_raw_socket("lo");
    init_terminal();
    struct pacote pacote;

    // Começando a executar
    char start = movimento();
    if (constroi_pacote(&pacote, sizeof(start), TIPO_INICIALIZACAO, (uint8_t *)&start))
        fprintf(stderr, "Erro ao construir pacote\n");
    fprintf(stderr, "\nMandando Start\n");
    rede_envia(&pacote, soq);

    // Enviando o mapa
    if (argc > 1)
    {
        fprintf(stderr, "\nMapa: %s\n", argv[1]);
        int tam_string = strlen(argv[1]) + 1;
        if (constroi_pacote(&pacote, tam_string, TIPO_TXT, (uint8_t *)argv[1]))
            fprintf(stderr, "Erro ao construir pacote\n");
        rede_envia(&pacote, soq);
        envia_csv(argv[1], soq);
        fprintf(stderr, "\nMandou o mapa\n");
    }
    else
    {
        if (constroi_pacote(&pacote, 0, TIPO_ERRO, NULL))
            fprintf(stderr, "Erro ao construir pacote\n");
        rede_envia(&pacote, soq);
    }

    while (1)
    {
        fprintf(stderr, "Ouvindo...\n");
        rede_escuta(&pacote, soq); // Esperando tamanho do mapa (visão)
        uint8_t lado = pacote.dados[0];
        fprintf(stderr, "\nDesenhando mapa\n");
        recebe_visao(soq, lado);

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

            if (pacote.tipo != TIPO_ERRO)
            {
                printf("\nMovimento Válido! %d\n", pacote.tipo);
                if (pacote.tipo == TIPO_FIM)
                {
                    fprintf(stderr, "\nEsperando arquivo fim\n");
                    arquivo_recebe(soq);
                    fprintf(stderr, "\nChegou fim\n");
                    return 0;
                }
                else if (pacote.tipo == TIPO_TXT || pacote.tipo == TIPO_JPG || pacote.tipo == TIPO_MP4)
                {
                    fprintf(stderr, "\nEsperando arquivo especial\n");
                    arquivo_recebe(soq);
                    fprintf(stderr, "\nChegou especial\n");
                    rede_escuta(&pacote, soq); // Escutando confirmação do movimento
                }
            }
            else
                fprintf(stderr, "\nMovimento invalido! Tente novamente.\n");
        } while (pacote.tipo == TIPO_ERRO);
    }

    close(soq);
    reset_terminal();
    return EXIT_SUCCESS;
}