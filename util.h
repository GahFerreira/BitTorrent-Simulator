#ifndef UTIL_H
#define UTIL_H

// Compara se dois `unsigned` são iguais.
bool comparar_unsigned(const unsigned *a, const unsigned *b);

// Sleep multi-plataforma.
void meu_sleep(unsigned milisegundos);

// Como não é thread-safe, chama-se somente uma vez.
void semear_numeros_aleatorios(void);
// Gera um número aleatório entre `minimo` e `maximo`, incluindo ambos.
unsigned aleatorio(unsigned minimo, unsigned maximo);

// Funções de regra de negócio.

// Assume que `destino` tenha a capacidade necessária para guardar o nome computado.
void id_usuario_para_nome_diretorio(char *destino, const unsigned id_usuario);

// Função que converte "fileX.Y" para o inteiro `X-1`.
// Presume-se que o nome do arquivo é SEMPRE "fileX.Y", tal que 'X' é um inteiro positivo.
unsigned nome_arquivo_para_id(const char nome_arquivo[]);

#endif // UTIL_H