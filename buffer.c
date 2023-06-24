#include <stdlib.h>

#include "buffer.h"

buffer_t *construir_buffer(const unsigned n_fragmentos, const unsigned tam_fragmento)
{
    buffer_t *novo_buffer = (buffer_t *) malloc(sizeof(buffer_t) + n_fragmentos * tam_fragmento);

    novo_buffer->n_fragmentos = n_fragmentos;
    novo_buffer->n_preenchidos = 0;

    return novo_buffer;
}