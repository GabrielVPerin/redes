#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <memory.h>

#include <rede.h>
#include <protocolo.h>

#define TIMEOUT_MAX 6

static size_t timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);

    return tp.tv_sec*1000 + tp.tv_usec/1000;
}
 
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

void rede_envia(struct pacote *pacote, int soquete)
{
    struct pacote resposta;
    int ret;
    size_t timeoutMilis = 300;
    size_t timeoutCont = 0;
    size_t comeco;
    struct timeval timeout;
    struct sockaddr_ll origem;
    socklen_t tamOrigem = sizeof(origem);

    while(timeoutCont <= TIMEOUT_MAX) {
        timeout.tv_sec = timeoutMilis/1000;
        timeout.tv_usec = (timeoutMilis % 1000) * 1000;
        setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));

        ret = send(soquete, pacote, sizeof(struct pacote), 0);
        if(ret == -1) {
            perror("Erro ao usar send");
            exit(1);
        }

        comeco = timestamp();

        while(1) {
            memset(&resposta, 2, sizeof(struct pacote));
            do {
                ret = recvfrom(soquete, &resposta, sizeof(struct pacote), 0, (struct sockaddr *) &origem, &tamOrigem); // Uso recvfrom para tratar pacotes duplicados pelo loopback
                if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    break;
                }
                else if(ret == -1) {
                    perror("Erro ao usar recv");
                    exit(1);
                }
            } while(resposta.marcador != MARCADOR && timestamp() - comeco <= timeoutMilis);

            if(origem.sll_pkttype == PACKET_OUTGOING)
                continue;

            if(!compara_crc(&resposta)) {
                if(resposta.tipo == TIPO_ACK) {
                    puts("ACK recebido!");
                    return;
                }
                else if(resposta.tipo == TIPO_NACK) {
                    puts("NACK recebido!");
                    break;
                }
            }
            
            if(timestamp() - comeco > timeoutMilis) {
                puts("Timeout!");
                timeoutCont++;
                timeoutMilis *= 2;
                break;
            }
        }
    }

    fprintf(stderr, "Quantidade máxima de timeouts excedida!\n");
    exit(1);
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

// Pergunta: Se um dos lados forem interrompidos o numero de sequencia será perdido, isso é um problema?

// Escuta um pacote e envia um ACK ou NACK caso o crc esteja certo ou não respectivamente
// IMPORTANTE: A função espera que a sequencia do pacote recebido seja a sequencia do pacote anterior + 1 mod 64
void rede_escuta(struct pacote *pacote, int soquete)
{
    uint8_t sequenciaAnterior = pacote->sequencia;
    struct sockaddr_ll origem;
    socklen_t tamOrigem = sizeof(origem);

    while(1) {
        do {
            if(recvfrom(soquete, pacote, sizeof(struct pacote), 0, (struct sockaddr *) &origem, &tamOrigem) == -1) { // Uso recvfrom para tratar pacotes duplicados pelo loopback
                perror("Erro ao usar recv");
                exit(1);
            }
        } while(pacote->marcador != MARCADOR);

        if(origem.sll_pkttype == PACKET_OUTGOING)
            continue;

        if(pacote->tipo != TIPO_NACK && pacote->tipo != TIPO_ACK && pacote->sequencia == (sequenciaAnterior + 1) % 64) {
            if(compara_crc(pacote)) {
                rede_envia_mensagem(soquete, TIPO_NACK);
            }
            else {
                rede_envia_mensagem(soquete, TIPO_ACK);

                return;
            }
        }
    }
}

