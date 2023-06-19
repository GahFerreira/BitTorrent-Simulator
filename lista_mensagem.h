/**
 * Estrutura de dados que armazena solicitações.
*/

#ifndef LISTA_MENSAGEM_H
#define LISTA_MENSAGEM_H

#include <stdbool.h>
#include <pthread.h>

#include "lista_encadeada.h"

typedef struct lista_mensagem lista_mensagem_t;

struct lista_mensagem
{
    lista_encadeada_t mensagens;
    pthread_mutex_t mutex_mensagem;
};

void inicializar_lista_mensagem(lista_mensagem_t *lista_mensagem);
void inicializar_multiplas_listas_mensagem(lista_mensagem_t **lista_listas_mensagem, const unsigned n_listas_mensagem);

// Manipulação da lista de mensagem, tratando acessos concorrentes.

void adicionar_elemento_lista_mensagens(lista_mensagem_t *lista_mensagem, const void *elemento);
void adicionar_elementos_lista_mensagens(lista_mensagem_t *lista_mensagem, const void **elementos, unsigned n_elementos);
// Remove o elemento buscado da lista e o retorna.
const void *extrair_elemento_lista_mensagens(lista_mensagem_t *lista_mensagem, const void *elemento, bool (funcao_comparacao) (const void *, const void *));
// Remove o primeiro elemento da lista e o retorna.
const void *extrair_primeiro_lista_mensagens(lista_mensagem_t *lista_mensagem);

void obter_dados_lista_mensagens(lista_mensagem_t *lista_mensagem, const void **destino);

#endif // LISTA_MENSAGEM_H