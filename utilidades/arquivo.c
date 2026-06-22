#include <stdio.h>
#include <string.h>

#include <protocolo.h>
#include <rede.h>

// Responsavel por enviar os dados do arquivo
static void envia_dados(FILE *arquivo, int soquete)
{
    struct pacote dados;
    uint8_t buffer[31];
    uint8_t tamBuffer = fread(buffer, sizeof(uint8_t), 31, arquivo);

    while(tamBuffer > 0) {
        constroi_pacote(&dados, tamBuffer, TIPO_DADOS, buffer);
        rede_envia(&dados, soquete);
        tamBuffer = fread(buffer, sizeof(uint8_t), 31, arquivo);
    }

    constroi_pacote(&dados, 0, TIPO_FIM, NULL);
    rede_envia(&dados, soquete);
}

// Responsavel por mandar o pacote de tipo de arquivo
static int arquivo_envia(char *nomeArquivo, int tipoPacote, int soquete)
{
    if(strlen(nomeArquivo) + 1 > MAX_6BIT)
        return 2;
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if(arquivo == NULL)
        return 1;

    struct pacote tipoArquivo;
    constroi_pacote(&tipoArquivo, strlen(nomeArquivo) + 1, tipoPacote, (uint8_t *) nomeArquivo);
    rede_envia(&tipoArquivo, soquete);
    envia_dados(arquivo, soquete);

    return 0;
}

// Cria um arquivo com o nome enviado e escreve o conteúdo
void arquivo_recebe(int soquete)
{
    struct pacote dados;
    rede_escuta(&dados, soquete);
    char nomeArquivo[dados.tamanho];
    strcpy(nomeArquivo, (char *) dados.dados);
    FILE *arquivo = fopen(nomeArquivo, "wb");

    do {
        rede_escuta(&dados, soquete);
        fwrite(dados.dados, sizeof(uint8_t), dados.tamanho, arquivo);
    } while(dados.tipo != TIPO_FIM);
}

// Envia arquivo MP4
// Retorna 1 caso o arquivo não exista
// Retorna 2 caso o nome do arquivo tenha mais que 63 caracteres
int envia_mp4(char *nomeArquivo, int soquete)
{
    return arquivo_envia(nomeArquivo, TIPO_MP4, soquete);
}

// Envia arquivo TXT
// Retorna 1 caso o arquivo não exista
// Retorna 2 caso o nome do arquivo tenha mais que 63 caracteres
int envia_txt(char *nomeArquivo, int soquete)
{
    return arquivo_envia(nomeArquivo, TIPO_TXT, soquete);
}

// Envia arquivo JPG
// Retorna 1 caso o arquivo não exista
// Retorna 2 caso o nome do arquivo tenha mais que 63 caracteres
int envia_jpg(char *nomeArquivo, int soquete)
{
    return arquivo_envia(nomeArquivo, TIPO_JPG, soquete);
}
