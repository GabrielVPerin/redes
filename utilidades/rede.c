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

#define MAX_TIMEOUT 6
#define TEMPO_TIMEOUT 300

// Retorna o tempo atual do seu computador em milisegundos
static size_t timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);

    return tp.tv_sec*1000 + tp.tv_usec/1000;
}
 
// Cria um socket em uma interface (função criada pelo Todt)
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

// Escuta um resposta de ACK/NACK e controla timeout
// Retorna 0 caso um ACK seja recebido
// Retorna 1 caso um NACK seja recebido
// Retorna 2 caso ocorra um timeout
static int escuta_resposta(int soquete, size_t timeoutMilis)
{
    struct pacote resposta;
    struct sockaddr_ll origem;
    socklen_t tamOrigem = sizeof(origem);
    int ret;
    size_t comeco = timestamp();

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

        if(timestamp() - comeco > timeoutMilis)
            return 2;

        if(!compara_crc(&resposta)) {
            if(resposta.tipo == TIPO_ACK)
                return 0;
            else if(resposta.tipo == TIPO_NACK)
                return 1;
        }
    }
}

static int byte_check(struct pacote_alternativo *pacote_alternativo ,struct pacote *pacote)
{
    if(pacote->tamanho > 8 && pacote->dados[8] == 0x81) { 
        memcpy(pacote_alternativo, pacote, 13);
        pacote_alternativo->dados[9] = 0xFF;
        memcpy((uint8_t *) pacote_alternativo + 14, (uint8_t *) pacote + 13, sizeof(struct pacote_alternativo) - 14);
        pacote->marcador = MARCADOR + 1;
        pacote_alternativo->crc = calcular_crc8((uint8_t *)pacote, sizeof(struct pacote) - 1);
        pacote->marcador = MARCADOR;

        return 1;
    }

    return 0;
}

// Fica enviando um pacote até que um ACK seja recebido ou o limite máximo de timeouts seja excedido
void rede_envia(struct pacote *pacote, int soquete)
{
    size_t timeoutMilis = TEMPO_TIMEOUT;
    size_t timeoutCont = 0;
    int ret;
    struct timeval timeout = {
        .tv_sec = timeoutMilis/1000,
        .tv_usec = (timeoutMilis % 1000) * 1000
    };
    setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));

    struct pacote_alternativo pacote_alternativo;
    int byteErro = byte_check(&pacote_alternativo, pacote);

    while(timeoutCont <= MAX_TIMEOUT) {
        if(byteErro == 1) {
            if(send(soquete, &pacote_alternativo, sizeof(struct pacote_alternativo), 0) == -1) {
                perror("Erro ao usar send (pacote_alternativo)");
                exit(1);
            }
        }
        else {
            if(send(soquete, pacote, sizeof(struct pacote), 0) == -1) {
                perror("Erro ao usar send");
                exit(1);
            }
        }

        ret = escuta_resposta(soquete, timeoutMilis);

        switch(ret) {
            case 0:
                puts("ACK recebido!");
                incrementa_sequencia();
                return;
            case 1:
                puts("NACK recebido!");
                break;
            case 2:
                timeoutCont++;
                timeoutMilis *= 2;
                puts("Timeout!");
                break;
        }
    }

    fprintf(stderr, "Quantidade máxima de timeouts excedida!\n");
    exit(1);
}

// Envia um pacote simples (sem dados)
static void rede_envia_mensagem(int soquete, uint8_t tipo)
{
    struct pacote mensagem;

    if(constroi_pacote(&mensagem, 0, tipo, NULL)) {
        fprintf(stderr, "Erro ao construir pacote");
        exit(1);
    }

    if(send(soquete, &mensagem, sizeof(struct pacote), 0) == -1) {
        perror("Erro ao usar send");
        exit(1);
    }

}

// Pergunta: Se um dos lados forem interrompidos o numero de sequencia será perdido, isso é um problema?
// TODO: Tenho que remover timeout na função escuta

// Escuta um pacote e envia um ACK ou NACK caso o crc esteja certo ou não respectivamente
// IMPORTANTE: A função espera que a sequencia do pacote recebido seja a sequencia do pacote anterior + 1 mod 64
void rede_escuta(struct pacote *pacote, int soquete)
{
    struct sockaddr_ll origem;
    socklen_t tamOrigem = sizeof(origem);

    struct timeval timeout = {
        .tv_sec = 0,
        .tv_usec = 0
    };
    setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));

    uint8_t buffer[sizeof(struct pacote_alternativo)];
    int ret;

    while(1) {
        memset(pacote, 0, sizeof(struct pacote));
        do {
            ret = recvfrom(soquete, buffer, sizeof(struct pacote_alternativo), 0, (struct sockaddr *) &origem, &tamOrigem);
            if(ret == -1) { // Uso recvfrom para tratar pacotes duplicados pelo loopback
                perror("Erro ao usar recv");
                exit(1);
            }
        } while(buffer[0] != MARCADOR && buffer[0] != MARCADOR + 1);

        if(origem.sll_pkttype == PACKET_OUTGOING)
            continue;

        if(buffer[0] == MARCADOR + 1) {
            memcpy(pacote, buffer, 13);
            memcpy((uint8_t *) pacote + 13, &buffer[14], ret - 14);
        }
        else {
            memcpy(pacote, buffer, sizeof(struct pacote));
        }

        if(pacote->tipo != TIPO_NACK && pacote->tipo != TIPO_ACK && pacote->sequencia == sequenciaGlobal) {
            if(compara_crc(pacote)) {
                rede_envia_mensagem(soquete, TIPO_NACK);
            }
            else {
                rede_envia_mensagem(soquete, TIPO_ACK);
                incrementa_sequencia();

                return;
            }
        }
    }
}

