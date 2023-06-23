#ifndef UTIL_H
#define UTIL_H

bool comparar_unsigned(const unsigned *a, const unsigned *b);

void meu_sleep(unsigned milisegundos);

// Como não é thread-safe, chama-se somente uma vez.
void semear_numeros_aleatorios(void);
// Gera um número aleatório entre `minimo` e `maximo`, incluindo ambos.
unsigned aleatorio(unsigned minimo, unsigned maximo);

#endif // UTIL_H