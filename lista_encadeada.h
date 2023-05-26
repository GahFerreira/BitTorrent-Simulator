#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

typedef struct no no_t;
typedef struct lista_encadeada lista_encadeada_t;

struct no
{
    no_t *proximo;
    void *dado;
};

struct lista_encadeada
{
    no_t *primeiro;
    no_t *ultimo;
};

#endif // LISTA_ENCADEADA_H
