/*
    Estrutura cujo propósito é guardar um dado genérico 
    que possui proteção contra acesso concorrente.
*/

#ifndef DADO_CONCORRENTE_H
#define DADO_CONCORRENTE_H

#include "pthread.h"

typedef struct dado_concorrente dado_concorrente_t;

struct dado_concorrente
{
    void *dado;
    pthread_mutex_t mutex_mensagem;
};

void inicializar_dado_concorrente(dado_concorrente_t *dado_concorrente, void *dado);

#endif // DADO_CONCORRENTE_H