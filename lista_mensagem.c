#include <pthread.h>

#include "lista_mensagem.h"
#include "lista_encadeada.h"

void inicializar_lista_mensagem(lista_mensagem_t *lista_mensagem)
{
    inicializar_lista_encadeada(&lista_mensagem->mensagens);
    lista_mensagem->mutex_mensagem = PTHREAD_MUTEX_INITIALIZER;
}