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
    inicia_programa(&pacote, soq);

    // Enviando o mapa
    carregando_mapa(argc, argv[1], &pacote, soq);

    while (1)
    {
        recebe_visao(soq, &pacote);

        do
        {
            fprintf(stderr, "\nEsperando movimento\n");
            enviar_movimento(&pacote, soq);
            printf("\nEscutando resposta\n");
            rede_escuta(&pacote, soq);

            if (pacote.tipo != TIPO_ERRO)
            {
                printf("\nMovimento Válido! %d\n", pacote.tipo);
                if (pacote.tipo == TIPO_FIM_DE_JOGO)
                {
                    fprintf(stderr, "\nChegou fim arquivos\n");
                    return 0;
                }
                if (pacote.tipo == TIPO_FIM)
                {
                    fprintf(stderr, "\nEsperando arquivo fim fantasma\n");
                    arquivo_recebe(soq, NULL);
                    fprintf(stderr, "\nChegou fim\n");
                    return 0;
                }
                else if (pacote.tipo == TIPO_TXT || pacote.tipo == TIPO_JPG || pacote.tipo == TIPO_MP4)
                {
                    fprintf(stderr, "\nEsperando arquivo especial\n");
                    char nomeArquivo[256]; // Ilegal?
                    arquivo_recebe(soq, nomeArquivo);
                    fprintf(stderr, "\nChegou especial\n");
                    abrir_midia(nomeArquivo);
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