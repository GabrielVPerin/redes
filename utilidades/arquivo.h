#ifndef ARQUIVO_H
#define ARQUIVO_H

// Envia arquivo MP4
// Retorna 1 caso o arquivo não exista
// Retorna 2 caso o nome do arquivo tenha mais que 63 caracteres
int envia_mp4(char *nomeArquivo, int soquete);

// Envia arquivo TXT
// Retorna 1 caso o arquivo não exista
// Retorna 2 caso o nome do arquivo tenha mais que 63 caracteres
int envia_txt(char *nomeArquivo, int soquete);

// Envia arquivo JPG
// Retorna 1 caso o arquivo não exista
// Retorna 2 caso o nome do arquivo tenha mais que 63 caracteres
int envia_jpg(char *nomeArquivo, int soquete);

int envia_csv(char *nomeArquivo, int soquete);

// Cria um arquivo com o nome enviado e escreve o conteúdo
void arquivo_recebe(int soquete, char *filename);

#endif
