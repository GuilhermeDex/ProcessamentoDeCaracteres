#ifndef INPUT_H
#define INPUT_H

FILE *abrir_arquivo(const char *nome_arquivo);
void fechar_arquivo(FILE *arquivo);
int ler_entrada(FILE *arquivo, int *M, int *T, char notas_melodia[][4], char notas_padrao[][4]);

#endif