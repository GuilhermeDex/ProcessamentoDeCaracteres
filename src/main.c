#include <stdio.h>
#include <stdlib.h>

#include "../include/logic.h"
#include "../include/input.h"



int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada> <algoritmo>\n", argv[0]);
        printf("Algoritmos disponiveis:\n");
        printf("1 - Força Bruta\n");
        printf("2 - KMP\n");
        printf("3 - BMH\n");
        printf("4 - Shift-And\n");
        return 1;
    }

    char *nome_arquivo = argv[1];
    int opcao = atoi(argv[2]);

    if (opcao < 1 || opcao > 4) {
        printf("Erro: Algoritmo invalido. Escolha entre 1 e 4.\n");
        return 1;
    }

    executar(nome_arquivo, opcao);
    return 0;
}
