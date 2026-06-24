#ifndef REDE_H
#define REDE_H

#include <protocolo.h>

// Cria um socket em uma interface (função criada pelo Todt)
int cria_raw_socket(char *nome_interface_rede);

// Fica enviando um pacote até que um ACK seja recebido ou o limite máximo de timeouts seja excedido
void rede_envia(struct pacote *pacote, int soquete);

// Escuta um pacote e envia um ACK ou NACK caso o crc esteja certo ou não respectivamente
// IMPORTANTE: A função espera que a sequencia do pacote recebido seja a sequencia do pacote anterior + 1 mod 64
void rede_escuta(struct pacote *pacote, int soquete);

void envia_visao(char **mapView, int lado, int soquete);

#endif
