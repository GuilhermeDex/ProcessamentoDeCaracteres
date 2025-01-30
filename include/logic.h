#ifndef LOGIC_H
#define LOGIC_H

//funcoes para a logica do musical

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