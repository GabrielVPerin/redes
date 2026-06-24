#include <stdint.h>
#include <memory.h>

#include <protocolo.h>

// Devo usar bit mask?

uint8_t sequenciaGlobal = 0;

// Incrementa 1 na variável sequenciaGlobal
void incrementa_sequencia()
{
    sequenciaGlobal = (sequenciaGlobal + 1) % (MAX_6BIT + 1);
}

uint8_t calcular_crc8(const uint8_t *dados, size_t tamanho) {
    uint8_t crc = 0;
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

// Constroi pacote
// Retorna 1 caso algum argumento exceda o valor máximo definido pelo protocolo e 0 caso contrário
int constroi_pacote(struct pacote *pacote, uint8_t tamanho, uint8_t tipo, const uint8_t *dados)
{
    if(tamanho > MAX_5BIT)
        return 1;
    if(tipo > MAX_5BIT)
        return 1;

    memset(pacote, 0, sizeof(struct pacote));

    pacote->marcador = MARCADOR;
    pacote->tamanho = tamanho;
    pacote->sequencia = sequenciaGlobal;
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
