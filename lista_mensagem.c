#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "lista_mensagem.h"
#include "lista_encadeada.h"

void inicializar_lista_mensagem(lista_mensagem_t *lista_mensagem)
{
    inicializar_lista_encadeada(&lista_mensagem->mensagens);
    lista_mensagem->mutex_mensagem = PTHREAD_MUTEX_INITIALIZER;
}

void inicializar_multiplas_listas_mensagem(lista_mensagem_t **listas_mensagem, const unsigned n_listas_mensagem)
{
    *listas_mensagem = (lista_mensagem_t *) calloc(n_listas_mensagem, sizeof(lista_mensagem_t));

    for (unsigned i_lista = 0; i_lista < n_listas_mensagem; ++i_lista)
    {
        inicializar_lista_mensagem(&(*listas_mensagem)[i_lista]);
    }
}

void adicionar_elemento_lista_mensagem(lista_mensagem_t *lista_mensagem, const void *elemento)
{
    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    adicionar_elemento_lista_encadeada(&lista_mensagem->mensagens, elemento);

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);
}

bool adicionar_elemento_unico_lista_mensagem(lista_mensagem_t *lista_mensagem, const void *elemento, bool (funcao_comparacao) (const void *, const void *))
{
    bool resultado = true;

    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    if (localizar_elemento_lista_encadeada(&lista_mensagem->mensagens, elemento, funcao_comparacao) == true) resultado = false;

    else adicionar_elemento_lista_encadeada(&lista_mensagem->mensagens, elemento);

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);

    return resultado;
}

void adicionar_elementos_lista_mensagem(lista_mensagem_t *lista_mensagem, const void **elementos, unsigned n_elementos)
{
    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    for (unsigned i_elemento = 0; i_elemento < n_elementos; ++i_elemento)
    {
        adicionar_elemento_lista_encadeada(&lista_mensagem->mensagens, elementos[i_elemento]);
    }

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);
}

const void *extrair_elemento_lista_mensagem(lista_mensagem_t *lista_mensagem, const void *elemento, bool (funcao_comparacao) (const void *, const void *))
{
    const void *resultado;

    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    resultado = extrair_elemento_lista_encadeada(&lista_mensagem->mensagens, elemento, funcao_comparacao);

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);

    // Fora do lock pois não afeta o estado de `lista_mensagem`.
    if (resultado == NULL) printf("[AVISO] Falha em extrair elemento de lista de mensagens. [lista_mensagem::extrair_elemento_lista_mensagem]\n\n");

    return resultado;
}

const void *extrair_primeiro_lista_mensagem(lista_mensagem_t *lista_mensagem)
{
    const void *resultado;

    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    resultado = extrair_primeiro_lista_encadeada(&lista_mensagem->mensagens);

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);

    #if DEBUG >= 8
    /*
        O comportamento de extrair elemento em lista vazia é esperado e usado em algumas funções.

        Está fora do lock pois não afeta o estado de `lista_mensagem`.
    */
    if (resultado == NULL) printf("[AVISO] Falha em extrair primeiro elemento de lista de mensagens. [lista_mensagem::extrair_primeiro_lista_mensagem]\n\n");
    #endif

    return resultado;
}

bool obter_primeiro_e_o_por_no_fim(lista_mensagem_t *lista_mensagem, void *destino, const unsigned tamanho)
{
    bool resultado = false;

    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);
    
    if (esta_vazia_lista_encadeada(&lista_mensagem->mensagens) == false)
    {
        resultado = true;

        const void *elemento = extrair_primeiro_lista_encadeada(&lista_mensagem->mensagens);

        memcpy(destino, elemento, tamanho);

        adicionar_elemento_lista_encadeada(&lista_mensagem->mensagens, elemento);
    }    

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);

    return resultado;
}

void obter_dados_lista_mensagem(lista_mensagem_t *lista_mensagem, const void **destino)
{
    pthread_mutex_lock(&lista_mensagem->mutex_mensagem);

    obter_dados_lista_encadeada(&lista_mensagem->mensagens, destino);

    pthread_mutex_unlock(&lista_mensagem->mutex_mensagem);
}