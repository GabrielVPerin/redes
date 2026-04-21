#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include <stdint.h>

#define MARCADOR 0x7E

struct pacote {
    uint8_t marcador;
    uint8_t tamanho;
    uint8_t sequencia;
    uint8_t tipo;
    uint8_t dados[31];
    uint8_t crc;
};

enum tipos_pacote {
    TIPO_ACK            = 0,
    TIPO_NACK           = 1,
    TIPO_VISUALIZACAO   = 2,
    TIPO_INICIALIZACAO  = 3,
    TIPO_DADOS          = 4,
    TIPO_TXT            = 5,
    TIPO_JPG            = 6,
    TIPO_MP4            = 7,
    TIPO_DIREITA        = 10,
    TIPO_ESQUERDA       = 11,
    TIPO_CIMA           = 12,
    TIPO_BAIXO          = 13,
    TIPO_ERRO           = 15,
    TIPO_FIM            = 16,
};


int constroi_pacote(struct pacote *pacote, uint8_t tamanho, uint8_t sequencia, uint8_t tipo, const uint8_t *dados);

#endif
