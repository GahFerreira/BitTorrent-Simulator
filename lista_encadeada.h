/**
 * Estrutura de dados de lista encadeada genérica.
*/

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

void inicializar_lista_encadeada(lista_encadeada_t *lista_encadeada);

#endif // LISTA_ENCADEADA_H
