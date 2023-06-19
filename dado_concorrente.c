#include "dado_concorrente.h"

void inicializar_dado_concorrente(dado_concorrente_t *dado_concorrente, void *dado)
{
    dado_concorrente->dado = dado;
    dado_concorrente->mutex_mensagem = PTHREAD_MUTEX_INITIALIZER;
}