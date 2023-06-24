#include <stdlib.h>

#include "buffer.h"

buffer_t *construir_buffer(const unsigned n_fragmentos, const unsigned tam_fragmento)
{
    buffer_t *novo_buffer = (buffer_t *) malloc(sizeof(buffer_t) + n_fragmentos * tam_fragmento);

    novo_buffer->n_fragmentos = n_fragmentos;
    novo_buffer->n_fragmentos_ausentes = n_fragmentos;

    novo_buffer->mutex_buffer = PTHREAD_MUTEX_INITIALIZER;

    return novo_buffer;
}

void trancar_buffer(buffer_t *buffer)
{
    pthread_mutex_lock(&buffer->mutex_buffer);
}

void destrancar_buffer(buffer_t *buffer)
{
    pthread_mutex_unlock(&buffer->mutex_buffer);
}