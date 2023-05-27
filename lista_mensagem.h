/**
 * Estrutura de dados que armazena solicitações.
*/

#ifndef LISTA_MENSAGEM_H
#define LISTA_MENSAGEM_H

#include <pthread.h>

#include "lista_encadeada.h"

typedef struct lista_mensagem lista_mensagem_t;

struct lista_mensagem
{
    lista_encadeada_t mensagens;
    pthread_mutex_t mutex_mensagem;
};

void inicializar_lista_mensagem(lista_mensagem_t *lista_mensagem);

#endif // LISTA_MENSAGEM_H