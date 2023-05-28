#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "lista_mensagem.h"
#include "lista_encadeada.h"

void inicializar_lista_mensagem(lista_mensagem_t *lista_mensagem)
{
    inicializar_lista_encadeada(&lista_mensagem->mensagens);
    lista_mensagem->mutex_mensagem = PTHREAD_MUTEX_INITIALIZER;
}

void inicializar_multiplas_listas_mensagem(lista_mensagem_t **listas_mensagem, unsigned n_listas_mensagem)
{
    *listas_mensagem = (lista_mensagem_t *) calloc(n_listas_mensagem,sizeof(lista_mensagem_t));

    for (unsigned i_lista = 0; i_lista < n_listas_mensagem; i_lista++)
    {
        inicializar_lista_mensagem(&(*listas_mensagem)[i_lista]);
    }
}

void adicionar_elemento_lista_mensagens(lista_mensagem_t *lista_mensagem, void *elemento)
{
    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    adicionar_elemento_lista_encadeada(&lista_mensagem->mensagens, elemento);

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);
}

bool remover_elemento_lista_mensagens(lista_mensagem_t *lista_mensagem, void *elemento, bool (funcao_comparacao) (void *, void *))
{
    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    bool resultado = remover_elemento_lista_encadeada(&lista_mensagem->mensagens, elemento, funcao_comparacao);

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);

    if (resultado == false)
    {
        printf("\nAVISO: Falha em remover elemento de lista de mensagens.\n\n");

        return false;
    }

    return true;
}