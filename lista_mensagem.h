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
void inicializar_multiplas_listas_mensagem(lista_mensagem_t **lista_listas_mensagem, unsigned n_listas_mensagem);

// Manipulação da lista de mensagem, tratando acessos concorrentes.

void adicionar_elemento_lista_mensagens(lista_mensagem_t *lista_mensagem, void *elemento);
bool remover_elemento_lista_mensagens(lista_mensagem_t *lista_mensagem, void *elemento, bool (funcao_comparacao) (void *, void *));

#endif // LISTA_MENSAGEM_H