#ifndef BUFFER_H
#define BUFFER_H

#include "pthread.h"

#define fragmento(buffer, i_fragmento) (buffer->dados_fragmentos[(i_fragmento * buffer->tam_fragmento)])

typedef unsigned char byte;
typedef struct buffer buffer_t;

struct buffer
{
    unsigned n_fragmentos, n_fragmentos_ausentes, tam_fragmento;

    pthread_mutex_t mutex_buffer;
    
    // Os bytes que comporão os fragmentos.
    byte dados_fragmentos[];
};

buffer_t *construir_buffer(const unsigned n_fragmentos, const unsigned tam_fragmento);

void trancar_buffer(buffer_t *buffer);
void destrancar_buffer(buffer_t *buffer);

#endif // BUFFER_H