#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input.h"

FILE *abrir_arquivo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
    }
    return arquivo;
}

void fechar_arquivo(FILE *arquivo) {
    fclose(arquivo);
}

int ler_entrada(FILE *arquivo, int *M, int *T, char notas_melodia[][4], char notas_padrao[][4]) {
    if (fscanf(arquivo, "%d %d", M, T) != 2 || (*M == 0 && *T == 0)) {
        return 0;
    }

    for (int i = 0; i < *M; i++) {
        fscanf(arquivo, "%s", notas_melodia[i]);
    }
    for (int i = 0; i < *T; i++) {
        fscanf(arquivo, "%s", notas_padrao[i]);
    }

    return 1;
}
