
#include "auxiliares.h"

void recebe_mapa(char mapa[MAP_SIZE][MAP_SIZE], struct pacote *pacote, int soq)
{
    rede_escuta(pacote, soq);
    if (pacote->tipo == TIPO_ERRO)
    {
        fprintf(stderr, "MAPA NULO\n");
        createMap(mapa, NULL);
    }
    else if (pacote->tipo == TIPO_TXT)
    {
        pacote->dados[pacote->tamanho - 1] = '\0';
        char *nomeMapa = (char *)pacote->dados;
        fprintf(stderr, "MAPA FORNECIDO: %s\n", nomeMapa);

        // receber o arquivo do mapa csv
        arquivo_recebe(soq, NULL); // da pra trocar o null por variavel com nome
        fprintf(stderr, "\nMapa chegou");
        createMap(mapa, nomeMapa);
        fprintf(stderr, "\nMapa criado");
    }
}

void envia_mapa_visivel(char mapa[MAP_SIZE][MAP_SIZE], struct pacman pacMan, struct pacote *pacote, int soq)
{
    char **mapView = drawPacmanView(mapa, pacMan);

    int lado = (int)pacMan.visao * 2 + 1;
    uint8_t lado_envio = (uint8_t)lado;

    if (constroi_pacote(pacote, sizeof(uint8_t), TIPO_VISUALIZACAO, &lado_envio))
        fprintf(stderr, "Erro ao construir pacote");
    rede_envia(pacote, soq);

    envia_visao(mapView, lado, soq);

    for (int i = 0; i < ((int)pacMan.visao * 2 + 1); i++)
        free(mapView[i]);
    free(mapView);
}

void zerou_jogo(struct pacote *pacote, int soq)
{
    fprintf(stderr, "FIM, acabou os arquivos\n");

    // envia pacote de aviso
    if (constroi_pacote(pacote, sizeof(char), TIPO_FIM_DE_JOGO, NULL))
        fprintf(stderr, "Erro ao construir pacote");

    rede_envia(pacote, soq);
    fprintf(stderr, "FIM, enviou\n");
}

void feedback_movimento(int moveP, struct pacote *pacote, int soq)
{
    if (moveP == -1)
    {
        fprintf(stderr, "Movimento Invalido\n");
        if (constroi_pacote(pacote, sizeof(moveP), TIPO_ERRO, (uint8_t *)&moveP))
            fprintf(stderr, "Erro ao construir pacote");
        fprintf(stderr, "Mandando Erro sobre o movimento\n");
        rede_envia(pacote, soq);
    }
    else // MOVIMENTO VALIDO
    {
        fprintf(stderr, "Movimento Valido\n");
        if (constroi_pacote(pacote, sizeof(moveP), TIPO_DADOS, (uint8_t *)&moveP))
            fprintf(stderr, "Erro ao construir pacote");
        fprintf(stderr, "Mandando movimento correto\n");
        rede_envia(pacote, soq);
    }
}