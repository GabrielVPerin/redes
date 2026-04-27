#include <stdint.h>
#include <memory.h>

#include <protocolo.h>

static uint8_t calcular_crc8(const uint8_t *dados, size_t tamanho) {
    uint8_t crc = 0x00;
    uint8_t polinomio = 0x07;

    for(size_t i = 0; i < tamanho; i++) {
        crc ^= dados[i];

        for(size_t j = 0; j < 8; j++) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ polinomio;
            } 
            else {
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

    memset(pacote, 0, sizeof(struct pacote));

    pacote->marcador = MARCADOR;
    pacote->tamanho = tamanho;
    pacote->sequencia = sequencia;
    pacote->tipo = tipo;
    if(dados != NULL)
        memcpy(pacote->dados, dados, tamanho);
    pacote->crc = calcular_crc8((uint8_t *)pacote, sizeof(struct pacote) - 1);

    return 0;
}

// Retorna 0 se forem iguais
// Retorna 1 se forem diferentes
int compara_crc(struct pacote *pacote)
{
    if(calcular_crc8((uint8_t *)pacote, sizeof(struct pacote) - 1) != pacote->crc)
        return 1;

    return 0;
}
