#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <arquivo.h>
#include <protocolo.h>
#include <rede.h>

void recebe_visao(int soq, struct pacote *pacoteOriginal)
{
    rede_escuta(pacoteOriginal, soq); // recebe tamanho do mapa (visão)
    uint8_t lado = pacoteOriginal->dados[0];
    int area = lado * lado;

    char *mapa = calloc(area, sizeof(char));
    int count = 0;
    struct pacote pacote;

    // descarta lixos (\n, \r, \0) enquanto escuta
    while (1)
    {
        rede_escuta(&pacote, soq);
        if (pacote.tipo == TIPO_FIM)
            break;

        if (pacote.tipo == TIPO_VISUALIZACAO)
        {
            for (int i = 0; i < pacote.tamanho; i++)
            {
                char ch = pacote.dados[i];
                if (ch != '\n' && ch != '\r' && ch != '\0' && count < area)
                {
                    mapa[count++] = ch;
                }
            }
        }
    }

    
    int topo = 0; // começo
    int fundo = lado - 1; // fim

    // acha a primeira linha que não é vazia
    while (topo < lado)
    {
        int vazia = 1;
        for (int j = 0; j < lado; j++)
        {
            char ch = mapa[topo * lado + j];
            if (ch != 'E' && ch != '0' && ch != 'V' && ch != ' ')
                vazia = 0;
        }
        if (!vazia)
            break;
        topo++;
    }

    // ultima linha que não é vazia
    while (fundo >= topo)
    {
        int vazia = 1;
        for (int j = 0; j < lado; j++)
        {
            char ch = mapa[fundo * lado + j];
            if (ch != 'E' && ch != '0' && ch != 'V' && ch != ' ')
                vazia = 0;
        }
        if (!vazia)
            break;
        fundo--;
    }

    // limpa a tela 
    printf("\033[2J\033[H");

    // imprime só as linhas com conteudo
    if (topo <= fundo)
    {
        for (int i = topo; i <= fundo; i++)
        {
            for (int j = 0; j < lado; j++)
            {
                char ch = mapa[i * lado + j];

                if (ch == 'X')
                    printf("# ");
                else if (ch == 'E' || ch == '0' || ch == 'V')
                    printf("  ");
                else
                    printf("%c ", ch);
            }
            printf("\n");
        }
    }

    fflush(stdout); // Força a atualização da tela
    free(mapa);
}

char movimento()
{
    return getchar();
}

struct termios oldt;
void reset_terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void init_terminal()
{
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void abrir_midia(const char *nome_arquivo)
{
    char comando[256];

    //  comando: xdg-open "nome_do_arquivo" > /dev/null 2>&1 &
    snprintf(comando, sizeof(comando), "xdg-open \"%s\" > /dev/null 2>&1 &", nome_arquivo);

    // Executa o comando no terminal
    system(comando);
}

void enviar_movimento(struct pacote *pacote, int soq)
{
    char move = movimento();
    int tipo;
    switch (move)
    {
    case 'w':
        tipo = TIPO_CIMA;
        break;
    case 'a':
        tipo = TIPO_ESQUERDA;
        break;
    case 's':
        tipo = TIPO_BAIXO;
        break;
    case 'd':
        tipo = TIPO_DIREITA;
        break;
    default:
        tipo = TIPO_ERRO;
        break;
    }

    if (constroi_pacote(pacote, sizeof(move), tipo, (uint8_t *)&move))
        fprintf(stderr, "Erro ao construir pacote\n");
    rede_envia(pacote, soq);
}

void inicia_programa(struct pacote *pacote, int soq)
{
    // Começando a executar
    char start = movimento();
    if (constroi_pacote(pacote, sizeof(start), TIPO_INICIALIZACAO, (uint8_t *)&start))
        fprintf(stderr, "Erro ao construir pacote\n");
    rede_envia(pacote, soq);
}

void carregando_mapa(int argc, char *argv, struct pacote *pacote, int soq)
{
    // Enviando o mapa
    if (argc > 1)
    {
        int tam_string = strlen(argv) + 1;
        if (constroi_pacote(pacote, tam_string, TIPO_TXT, (uint8_t *)argv))
            fprintf(stderr, "Erro ao construir pacote\n");
        rede_envia(pacote, soq);
        envia_csv(argv, soq);
    }
    else
    {
        if (constroi_pacote(pacote, 0, TIPO_ERRO, NULL))
            fprintf(stderr, "Erro ao construir pacote\n");
        rede_envia(pacote, soq);
    }
}