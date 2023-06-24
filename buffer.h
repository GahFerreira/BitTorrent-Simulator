#ifndef BUFFER_H
#define BUFFER_H

typedef unsigned char byte;
typedef struct buffer buffer_t;

struct buffer
{
    unsigned n_fragmentos, n_preenchidos;
    
    byte fragmentos[];
};

buffer_t *construir_buffer(const unsigned n_fragmentos, const unsigned tam_fragmento);

#endif // BUFFER_H