#ifndef __CLIENT__
#define __CLIENT__

void recebe_visao(int soq, struct pacote *pacoteOriginal);
char movimento();
void reset_terminal();
void init_terminal();
void abrir_midia(const char *nome_arquivo);
void enviar_movimento(struct pacote *pacote, int soq);
void inicia_programa(struct pacote *pacote, int soq);
void carregando_mapa(int argc, char *argv, struct pacote *pacote, int soq);

#endif
