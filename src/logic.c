#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/logic.h"
#include "../include/input.h"
#include "../include/timer.h"

#define MAX_NOTES 10000
#define SEMITONES 12
#define MAX_BITS (sizeof(unsigned long long) * 8)
#define MAX_SEGMENTS 4  // do jeito que esta aqui permite ate 256 bits de padrao

typedef unsigned long long ull;

// criando uma estrutura para armazenar padroes maiores que 64 bits
typedef struct {
    ull segments[MAX_SEGMENTS];
} Bitset;


// criando 2 funcoes auxiliares  para facilitar a manipulacao do bitset
void bitset_clear(Bitset *b) {
    memset(b->segments, 0, sizeof(b->segments));
}

void bitset_set(Bitset *b, int pos) {
    int idx = pos / MAX_BITS;
    int bit = pos % MAX_BITS;
    b->segments[idx] |= (1ULL << bit);
}

// tabela de mapeamento de notas para indices
int nota_para_indice(char *nota) {
    if (strcmp(nota, "C") == 0) return 0;
    if (strcmp(nota, "C#") == 0 || strcmp(nota, "Db") == 0) return 1;
    if (strcmp(nota, "D") == 0) return 2;
    if (strcmp(nota, "D#") == 0 || strcmp(nota, "Eb") == 0) return 3;
    if (strcmp(nota, "E") == 0) return 4;
    if (strcmp(nota, "F") == 0) return 5;
    if (strcmp(nota, "F#") == 0 || strcmp(nota, "Gb") == 0) return 6;
    if (strcmp(nota, "G") == 0) return 7;
    if (strcmp(nota, "G#") == 0 || strcmp(nota, "Ab") == 0) return 8;
    if (strcmp(nota, "A") == 0) return 9;
    if (strcmp(nota, "A#") == 0 || strcmp(nota, "Bb") == 0) return 10;
    if (strcmp(nota, "B") == 0) return 11;
    return -1;
}

void converter_para_indices(char notas[][4], int indices[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        indices[i] = nota_para_indice(notas[i]);
    }
}

void normalizar(int *notas, int tamanho, int deslocamento) {
    for (int i = 0; i < tamanho; i++) {
        notas[i] = (notas[i] + deslocamento) % SEMITONES;
    }
}

// Forca Bruta
int forca_bruta(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao) {
    for (int i = 0; i <= tamanho_texto - tamanho_padrao; i++) {
        int casou = 1;
        for (int j = 0; j < tamanho_padrao; j++) {
            if (texto[i + j] != padrao[j]) {
                casou = 0;
                break;
            }
        }
        if (casou) {
            return i;
        }
    }
    return -1;
}

// KMP
void construir_tabela_kmp(int *padrao, int tamanho_padrao, int *tabela) {
    tabela[0] = 0;
    int j = 0;
    for (int i = 1; i < tamanho_padrao; i++) {
        if (padrao[i] == padrao[j]) {
            j++;
            tabela[i] = j;
        } else {
            if (j != 0) {
                j = tabela[j - 1];
                i--;
            } else {
                tabela[i] = 0;
            }
        }
    }
}

int kmp(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao) {
    int tabela[tamanho_padrao];
    construir_tabela_kmp(padrao, tamanho_padrao, tabela);

    int i = 0, j = 0;
    while (i < tamanho_texto) {
        if (texto[i] == padrao[j]) {
            i++;
            j++;
        }
        if (j == tamanho_padrao) {
            return i - j;
        } else if (i < tamanho_texto && texto[i] != padrao[j]) {
            if (j != 0) {
                j = tabela[j - 1];
            } else {
                i++;
            }
        }
    }
    return -1;
}

// BMH
int bmh(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao) {
    int bad_char[SEMITONES];
    for (int i = 0; i < SEMITONES; i++) {
        bad_char[i] = tamanho_padrao;
    }
    for (int i = 0; i < tamanho_padrao - 1; i++) {
        bad_char[padrao[i]] = tamanho_padrao - 1 - i;
    }

    int deslocamento = 0;
    while (deslocamento <= tamanho_texto - tamanho_padrao) {
        int j = tamanho_padrao - 1;
        while (j >= 0 && padrao[j] == texto[deslocamento + j]) {
            j--;
        }
        if (j < 0) {
            return deslocamento;
        }
        deslocamento += bad_char[texto[deslocamento + tamanho_padrao - 1]];
    }
    return -1;
}


// SHIFT_AND 
int shift_and(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao) {
    Bitset Mascara[SEMITONES];  
    Bitset R;                   
    int i;                      

// verificando se o tamnaho do padrao excede o limite
    if (tamanho_padrao > MAX_BITS * MAX_SEGMENTS) {
        printf("Erro: Tamanho do padrao maior que o suportado (%ld bits).\n", MAX_BITS * MAX_SEGMENTS);
        return -99;
    }

    memset(Mascara, 0, sizeof(Mascara));
    bitset_clear(&R);

    for (i = 0; i < tamanho_padrao; i++) {
        if (padrao[i] < 0 || padrao[i] >= SEMITONES) {
            printf("Erro: Nota invalida no padrao na posicao %d (valor: %d).\n", i, padrao[i]);
            return -1;
        }
        bitset_set(&Mascara[padrao[i]], tamanho_padrao - i - 1);                                   //define os bits correspondentes ao padrao
    }

    for (i = 0; i < tamanho_texto; i++) {
        if (texto[i] < 0 || texto[i] >= SEMITONES) {
            printf("Erro: Nota invalida no texto na posicao %d (valor: %d).\n", i, texto[i]);
            return -1;
        }

        Bitset temp;
        memcpy(&temp, &R, sizeof(Bitset));

        for (int j = MAX_SEGMENTS - 1; j >= 0; j--) {
            ull carry = (j == 0) ? 1ULL << (tamanho_padrao - 1) : (R.segments[j - 1] & 1ULL) << (MAX_BITS - 1);
            temp.segments[j] = (R.segments[j] >> 1) | carry;
            temp.segments[j] &= Mascara[texto[i]].segments[j];
        }

        memcpy(&R, &temp, sizeof(Bitset));

        if (R.segments[0] & 1ULL) {                                                                   // verifica a correspondencia do padrao foi encontrada
            return (i - tamanho_padrao + 1);
        }
    }

    return -1;
}





void executar(const char *nome_arquivo, int opcao) {
    int M, T;
    char notas_melodia[MAX_NOTES][4], notas_padrao[MAX_NOTES][4];
    int indices_melodia[MAX_NOTES], indices_padrao[MAX_NOTES];

    FILE *arquivo = abrir_arquivo(nome_arquivo);
    if (!arquivo) {
        return;
    }

    FILE *saida = fopen("resultado.txt", "w");
    if (!saida) {
        perror("Erro ao criar o arquivo de saida");
        fechar_arquivo(arquivo);
        return;
    }


    while (ler_entrada(arquivo, &M, &T, notas_melodia, notas_padrao)) {
        converter_para_indices(notas_melodia, indices_melodia, M);
        converter_para_indices(notas_padrao, indices_padrao, T);

        int resultado = -1;

        start_timer();

        for (int deslocamento = 0; deslocamento < SEMITONES; deslocamento++) {
            int padrao_deslocado[T];
            memcpy(padrao_deslocado, indices_padrao, T * sizeof(int));
            normalizar(padrao_deslocado, T, deslocamento);

            switch (opcao) {
                case 1:
                    resultado = forca_bruta(indices_melodia, M, padrao_deslocado, T);
                    break;
                case 2:
                    resultado = kmp(indices_melodia, M, padrao_deslocado, T);
                    break;
                case 3:
                    resultado = bmh(indices_melodia, M, padrao_deslocado, T);
                    break;
                case 4:
                    resultado = shift_and(indices_melodia, M, padrao_deslocado, T);
                    break;
                default:
                    fprintf(saida, "Erro interno: Algoritmo invalido.\n");
                    printf("Erro interno: Algoritmo invalido.\n");
                    fechar_arquivo(arquivo);
                    fclose(saida);
                    return;
            }

            if (resultado != -1) {
                break;
            }
        }

        stop_timer();

        if (resultado != -1) {
            printf("S %d\n", resultado);
            fprintf(saida, "S %d\n", resultado);
        } else {
            printf("N\n");
            fprintf(saida, "N\n");
        }

        print_timer();
        print_timer_to_file(saida);
    }

    fechar_arquivo(arquivo);
    fclose(saida);

    printf("Resultado salvo no arquivo 'resultado.txt'.\n");
}



