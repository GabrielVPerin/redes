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
    fprintf(stderr, "Ouvindo...\n");
    rede_escuta(pacoteOriginal, soq); // Esperando tamanho do mapa (visão)
    uint8_t lado = pacoteOriginal->dados[0];
    fprintf(stderr, "\nDesenhando mapa\n");

    system("clear");
    struct pacote pacote;
    int total = (lado * lado * 2) + 10;
    char *buffer = calloc(total, sizeof(char));
    int pos = 0;

    while (1)
    {
        rede_escuta(&pacote, soq);
        if (pacote.tipo == TIPO_FIM)
            break;

        if (pacote.tipo == TIPO_VISUALIZACAO)
        {
            int copy_len = pacote.tamanho;
            if (pos + copy_len > total)
                copy_len = total - pos;
            memcpy(buffer + pos, pacote.dados, copy_len);
            pos += copy_len;
        }
    }

    int col = 0;
    for (int i = 0; i < pos; i++)
    {
        char c = buffer[i];
        if (c == '\n')
        {
            if (col != 0)
            {
                printf("\n");
                col = 0;
            }
            continue;
        }

        if (c == '\r' || c == '\0')
            continue;

        switch (c)
        {
        case 'X':
            printf("# ");
            break;
        case 'E':
        case '0':
        case 'V':
            printf("  ");
            break;
        default:
            // imprime P, fantasmas e arquivos
            printf("%c ", c);
            break;
        }
        col++;

        if (col == lado)
        {
            printf("\n");
            col = 0;
        }
    }

    if (col != 0)
        printf("\n");

    free(buffer);
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
    printf("\nMandando move\n");
    rede_envia(pacote, soq);
}

void inicia_programa(struct pacote *pacote, int soq)
{
    // Começando a executar
    char start = movimento();
    if (constroi_pacote(pacote, sizeof(start), TIPO_INICIALIZACAO, (uint8_t *)&start))
        fprintf(stderr, "Erro ao construir pacote\n");
    fprintf(stderr, "\nMandando Start\n");
    rede_envia(pacote, soq);
}

void carregando_mapa(int argc, char *argv, struct pacote *pacote, int soq)
{
    // Enviando o mapa
    if (argc > 1)
    {
        fprintf(stderr, "\nMapa: %s\n", argv);
        int tam_string = strlen(argv) + 1;
        if (constroi_pacote(pacote, tam_string, TIPO_TXT, (uint8_t *)argv))
            fprintf(stderr, "Erro ao construir pacote\n");
        rede_envia(pacote, soq);
        envia_csv(argv, soq);
        fprintf(stderr, "\nMandou o mapa\n");
    }
    else
    {
        if (constroi_pacote(pacote, 0, TIPO_ERRO, NULL))
            fprintf(stderr, "Erro ao construir pacote\n");
        rede_envia(pacote, soq);
    }
}