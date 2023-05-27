#include <stdlib.h>

#include "lista_encadeada.h"

void inicializar_lista_encadeada(lista_encadeada_t *lista_encadeada)
{
    lista_encadeada->primeiro = NULL;
    lista_encadeada->ultimo = NULL;
}