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
    const void *dado;
};

struct lista_encadeada
{
    no_t *primeiro;
    no_t *ultimo;
    unsigned tamanho;
};

void inicializar_no(no_t *no, const void *dado);
void inicializar_lista_encadeada(lista_encadeada_t *lista_encadeada);

bool esta_vazia_lista_encadeada(const lista_encadeada_t *lista_encadeada);
bool localizar_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, const void *elemento, bool (funcao_comparacao) (const void *, const void *));
void adicionar_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, const void *elemento);
const void *extrair_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, const void *elemento, bool (funcao_comparacao) (const void *, const void *));
const void *extrair_primeiro_lista_encadeada(lista_encadeada_t *lista_encadeada);

/*
Obtém todos os dados da lista encadeada e os coloca no vetor `destino`.
Presume que `destino` tenha a capacidade necessária para receber os dados da lista.

Contra-intuitivamente, apesar de `destino` ser `const`, o vetor apontado por `destino` não é
`const`: assim, é possível mudar o valor dos elementos de `destino`, mas não para onde
aponta `destino`.

Para não ser possível mudar os elementos apontados por destino, o tipo deveria ser algo como:
[const void * const *].
*/
void obter_dados_lista_encadeada(const lista_encadeada_t *lista_encadeada, const void **destino);

#endif // LISTA_ENCADEADA_H
