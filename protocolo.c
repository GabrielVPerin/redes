#include <stdint.h>
#include <memory.h>

#include "protocolo.h"

#define BIT5MASK 0x1F
#define BIT6MASK 0x3F

static uint8_t calcular_crc8(const uint8_t *dados, size_t tamanho) {
    uint8_t crc = 0x00; // Valor inicial
    uint8_t polinomio = 0x07; // Polinômio gerador

    for (size_t i = 0; i < tamanho; i++) {
        crc ^= dados[i]; // Faz o XOR do byte atual com o CRC

        // Para cada um dos 8 bits do byte
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) { // Se o bit mais à esquerda for 1
                crc = (crc << 1) ^ polinomio;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

int constroi_pacote(struct pacote *pacote, uint8_t tamanho, uint8_t sequencia, uint8_t tipo, const uint8_t *dados)
{
    if(tamanho > 31)
        return 1;
    if(sequencia > 63)
        return 1;
    if(tipo > 31)
        return 1;

    pacote->marcador = MARCADOR;
    pacote->tamanho = tamanho & BIT5MASK;
    pacote->sequencia = sequencia & 0x3F;
    pacote->tipo = tipo;
    memcpy(pacote->dados, dados, tamanho);
    pacote->crc = calcular_crc8((uint8_t *)pacote, sizeof(struct pacote) - 1);

    return 0;
}
