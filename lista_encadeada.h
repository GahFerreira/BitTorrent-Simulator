/**
 * Estrutura de dados de lista encadeada genérica.
 *
 * Uma premissa importante desta implementação é que NÃO haverá necessidade de
 * desalocar o conteúdo de `dado`.
*/

#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

#include <stdbool.h>

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

void inicializar_no(no_t *no, void *dado);
void inicializar_lista_encadeada(lista_encadeada_t *lista_encadeada);
bool esta_vazia_lista_encadeada(lista_encadeada_t *lista_encadeada);
void adicionar_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, void *elemento);
bool remover_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, void *elemento, bool (funcao_comparacao) (void *, void *));

#endif // LISTA_ENCADEADA_H
