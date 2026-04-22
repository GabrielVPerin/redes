#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "rede.h"
#include "protocolo.h"
 
int cria_raw_socket(char *nome_interface_rede) 
{
    // Cria arquivo para o socket sem qualquer protocolo
    int soquete = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(soquete == -1) {
        fprintf(stderr, "Erro ao criar socket: Verifique se você é root!\n");
        exit(1);
    }
 
    int ifindex = if_nametoindex(nome_interface_rede);
 
    struct sockaddr_ll endereco = {0};
    endereco.sll_family = AF_PACKET;
    endereco.sll_protocol = htons(ETH_P_ALL);
    endereco.sll_ifindex = ifindex;
    // Inicializa socket
    if(bind(soquete, (struct sockaddr*) &endereco, sizeof(endereco)) == -1) {
        fprintf(stderr, "Erro ao fazer bind no socket\n");
        exit(1);
    }
 
    struct packet_mreq mr = {0};
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    // Não joga fora o que identifica como lixo: Modo promíscuo
    if(setsockopt(soquete, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        fprintf(stderr, "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente.\n");
        exit(1);
    }
 
    return soquete;
}

// TODO: Talvez send pode enviar pacotes parciais
// TODO: Timeout

void rede_envia(struct pacote *pacote, int soquete)
{
    struct pacote resposta;

    while(1) {
        if(send(soquete, pacote, sizeof(struct pacote), 0) == -1) {
            perror("Erro ao usar send");
            exit(1);
        }

        while(1) {
            do {
                if(recv(soquete, &resposta, sizeof(struct pacote), 0) == -1) {
                    perror("Erro ao usar recv");
                    exit(1);
                }
            } while(resposta.marcador != MARCADOR);

            if(resposta.tipo == TIPO_ACK) {
                puts("ACK recebido!");
                return;
            }
            else if(resposta.tipo == TIPO_NACK) {
                puts("NACK recebido!");
                break;
            }
        }
    }
}

static void rede_envia_mensagem(int soquete, uint8_t codigo)
{
    struct pacote mensagem;

    if(constroi_pacote(&mensagem, 0, 0, codigo, NULL)) {
        fprintf(stderr, "Erro ao construir pacote");
        exit(1);
    }

    if(send(soquete, &mensagem, sizeof(struct pacote), 0) == -1) {
        perror("Erro ao usar send");
        exit(1);
    }

}

void rede_escuta(struct pacote *pacote, int soquete)
{
    while(1) {
        do {
            if(recv(soquete, pacote, sizeof(struct pacote), 0) == -1) {
                perror("Erro ao usar recv");
                exit(1);
            }
        } while(pacote->marcador != MARCADOR);

        if(compara_crc(pacote)) {
            rede_envia_mensagem(soquete, TIPO_NACK);
        }
        else {
            rede_envia_mensagem(soquete, TIPO_ACK);

            return;
        }

    }
}

