#include <stdio.h>
#include <string.h>

#include <protocolo.h>
#include <rede.h>

static void arquivo_envia(FILE *arquivo, int soquete)
{
    struct pacote dados;
    uint8_t buffer[31];
    uint8_t tamBuffer;

    do {
        tamBuffer = fread(buffer, sizeof(uint8_t), 31, arquivo);
        constroi_pacote(&dados, tamBuffer, TIPO_DADOS, buffer);
        rede_envia(&dados, soquete);
    } while(tamBuffer != 0);

    constroi_pacote(&dados, 0, TIPO_FIM, NULL);
    rede_envia(&dados, soquete);
}

void arquivo_recebe(FILE *arquivo, int soquete)
{
    struct pacote dados;
    rede_escuta(&dados, soquete);
    while(dados.tipo != TIPO_FIM) {
        fwrite(dados.dados, sizeof(uint8_t), dados.tamanho, arquivo);
        rede_escuta(&dados, soquete);
    }
}

int envia_mp4(char *nomeArquivo, int soquete)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    struct pacote tipoArquivo;
    if(strlen(nomeArquivo) + 1 > MAX_6BIT)
        return -1;
    constroi_pacote(&tipoArquivo, strlen(nomeArquivo) + 1, TIPO_MP4, (uint8_t *) nomeArquivo);
    rede_envia(&tipoArquivo, soquete);
    arquivo_envia(arquivo, soquete);

    return 0;
}
