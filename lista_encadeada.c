#include <stdlib.h>
#include <stdbool.h>

#include "lista_encadeada.h"

void inicializar_no(no_t *no, void *dado)
{
    no->proximo = NULL;
    no->dado = dado;
}

void inicializar_lista_encadeada(lista_encadeada_t *lista_encadeada)
{
    lista_encadeada->primeiro = NULL;
    lista_encadeada->ultimo = NULL;
}

bool esta_vazia_lista_encadeada(lista_encadeada_t *lista_encadeada)
{
    if (lista_encadeada->primeiro == NULL) return true;
    return false;
}

void adicionar_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, void *elemento)
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
}

// Retorna `true` se a remoção foi feita com sucesso, e `false` caso contrário.
bool remover_elemento_lista_encadeada(lista_encadeada_t *lista_encadeada, void *elemento, bool (funcao_comparacao) (void *, void *))
{
    if (esta_vazia_lista_encadeada(lista_encadeada) == true) return false;

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

            free(atual);

            return true;
        }

        // Se o nó atual é o último, e o `if` não entrou nenhuma vez, o elem. não está na lista.
        if (atual->proximo == NULL) return false;

        anterior = atual;
        atual = atual->proximo;
    }
}