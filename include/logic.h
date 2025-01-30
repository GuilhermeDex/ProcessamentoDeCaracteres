#ifndef LOGIC_H
#define LOGIC_H

#define MAX_SEGMENTS 4  // do jeito que esta aqui permite ate 256 bits de padrao
#define MAX_NOTES 10000
#define SEMITONES 12
#define MAX_BITS (sizeof(unsigned long long) * 8)

typedef unsigned long long ull;


// criando uma estrutura para armazenar padroes maiores que 64 bits
typedef struct {
    ull segments[MAX_SEGMENTS];
} Bitset;


//funcoes para a logica do musical

void bitset_clear(Bitset *b);
void bitset_set(Bitset *b, int pos);
int nota_para_indice(char *nota);
void converter_para_indices(char notas[][4], int indices[], int tamanho);
void normalizar(int *notas, int tamanho, int deslocamento);
int forca_bruta(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao);
void construir_tabela_kmp(int *padrao, int tamanho_padrao, int *tabela);
int kmp(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao);
int bmh(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao);
int shift_and(int *texto, int tamanho_texto, int *padrao, int tamanho_padrao);
void executar(const char *nome_arquivo, int opcao);



#endif