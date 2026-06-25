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

    printf("\nPressione qualquer tecla para iniciar a comunicação\n");
    // Começando a executar
    inicia_programa(&pacote, soq);

    // Enviando o mapa
    carregando_mapa(argc, argv[1], &pacote, soq);

    recebe_visao(soq, &pacote);
    while (1)
    {

        do
        {
            enviar_movimento(&pacote, soq);
            rede_escuta(&pacote, soq);

            if (pacote.tipo != TIPO_ERRO)
            {
                if (pacote.tipo == TIPO_FIM_DE_JOGO) // Quando zera o jogo
                    return 0;

                if (pacote.tipo == TIPO_FIM) // Quando morre pra fantasma
                {
                    char nomeArquivo[256];
                    recebe_visao(soq, &pacote);
                    arquivo_recebe(soq, nomeArquivo);
                    abrir_midia(nomeArquivo);
                    return 0;
                }
                else if (pacote.tipo == TIPO_TXT || pacote.tipo == TIPO_JPG || pacote.tipo == TIPO_MP4)
                {
                    fprintf(stderr, "\nEsperando arquivo especial\n");
                    char nomeArquivo[256];
                    arquivo_recebe(soq, nomeArquivo);
                    abrir_midia(nomeArquivo);
                    rede_escuta(&pacote, soq); // Escutando confirmação do movimento
                }
            }
        } while (pacote.tipo == TIPO_ERRO);

        recebe_visao(soq, &pacote);
    }

    close(soq);
    reset_terminal();
    return EXIT_SUCCESS;
}