#include <stdlib.h>
#include <stdbool.h>

#include "lista_encadeada.h"

void inicializar_no(no_t *no, const void *dado)
{
    no->proximo = NULL;
    no->dado = dado;
}

void inicializar_lista_encadeada(lista_encadeada_t *lista_encadeada)
{
    lista_encadeada->primeiro = NULL;
    lista_encadeada->ultimo = NULL;
    lista_encadeada->tamanho = 0;
}

bool esta_vazia_lista_encadeada(const lista_encadeada_t *lista_encadeada)
{
    if (lista_encadeada->tamanho == 0) return true;
    return false;
}

void adicionar_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, const void *elemento)
{
    no_t *novo_no = (no_t *) calloc(1, sizeof(no_t));
    inicializar_no(novo_no, elemento);

    if (esta_vazia_lista_encadeada(lista_encadeada) == true)
    {
        lista_encadeada->primeiro = novo_no;
    }

    else
    {
        lista_encadeada->ultimo->proximo = novo_no;
    }

    lista_encadeada->ultimo = novo_no;
    ++lista_encadeada->tamanho;
}

// Retorna `true` se a remoção foi feita com sucesso, e `false` caso contrário.
const void *extrair_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, const void *elemento, bool (funcao_comparacao) (const void *, const void *))
{
    if (esta_vazia_lista_encadeada(lista_encadeada) == true) return NULL;

    /**
     * A implementação abaixo resolve todos os seguintes casos:
     * 
     * Em relação à posição do elemento removido:
     *   1.1. Remover primeiro elemento.
     *   1.2. Remover ultimo elemento.
     *   1.3. Remover outro elemento.
     * 
     * Em relação a como a estrutura fica após:
     *   2.1. A estrutura possui 0 elementos.
     *   2.2. A estrutura possui 1 elemento.
     *   2.3. A estrutura possui 2 ou mais elementos.
    */

    no_t *atual = lista_encadeada->primeiro;
    no_t *anterior = NULL;

    while (true)
    {
        if (funcao_comparacao(elemento, atual->dado) == true)
        {
            // Se for o último elemento, o novo último é o anterior.
            if (atual->proximo == NULL) lista_encadeada->ultimo = anterior;

            // Se for o primeiro elemento, o novo primeiro é o seu próximo.
            if (anterior == NULL) lista_encadeada->primeiro = atual->proximo;
            
            // Se não for o primeiro elemento, faz o anterior apontar para o próximo.
            else anterior->proximo = atual->proximo;

            const void *resultado = atual->dado;

            free(atual);
            lista_encadeada->tamanho--;

            return resultado;
        }

        // Se o nó atual é o último, e o `if` não entrou nenhuma vez, o elem. não está na lista.
        if (atual->proximo == NULL) return NULL;

        anterior = atual;
        atual = atual->proximo;
    }
}

const void *extrair_primeiro_lista_encadeada(lista_encadeada_t *lista_encadeada)
{
    if (esta_vazia_lista_encadeada(lista_encadeada) == true) return NULL;

    const void *resultado = lista_encadeada->primeiro->dado;

    no_t *atual = lista_encadeada->primeiro;

    // Se for o último elemento, o novo último é o anterior.
    if (atual->proximo == NULL) lista_encadeada->ultimo = NULL;

    // Se for o primeiro elemento, o novo primeiro é o seu próximo.
    lista_encadeada->primeiro = atual->proximo;

    free(atual);
    lista_encadeada->tamanho--;

    return resultado;
}

void obter_dados_lista_encadeada(const lista_encadeada_t *lista_encadeada, const void **destino)
{
    if (esta_vazia_lista_encadeada(lista_encadeada) == true) return;

    no_t *atual = lista_encadeada->primeiro;
    unsigned i_no = 0;
    while (true)
    {
        destino[i_no] = atual->dado;

        if (atual->proximo == NULL) break;
        atual = atual->proximo;
        ++i_no;
    }
}