#ifndef REDE_H
#define REDE_H

#include <protocolo.h>

int cria_raw_socket(char *nome_interface_rede);

void rede_envia(struct pacote *pacote, int soquete);

void rede_escuta(struct pacote *pacote, int soquete);

#endif
