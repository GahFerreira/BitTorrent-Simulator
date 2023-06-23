#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "util.h"

bool comparar_unsigned(const unsigned *a, const unsigned *b)
{
    if (*a == *b) return true;
    return false;
}

void meu_sleep(unsigned milisegundos)
{
    #ifdef _WIN32
    Sleep(milisegundos);
    #else
    sleep(milisegundos / 1000);
    #endif
}

void semear_numeros_aleatorios(void)
{
    srand((unsigned) time(NULL));
}

unsigned aleatorio(unsigned minimo, unsigned maximo)
{
    return minimo + ( (unsigned) rand() % (maximo-minimo+1) );
}