#include <stdlib.h>

#include "buffer.h"

buffer_t *construir_buffer(const unsigned n_fragmentos, const unsigned tam_fragmento)
{
    buffer_t *novo_buffer = (buffer_t *) malloc(sizeof(buffer_t) + (n_fragmentos * tam_fragmento * sizeof(byte)));

    novo_buffer->n_fragmentos = n_fragmentos;
    novo_buffer->tam_fragmento = tam_fragmento;

    /*
        O número de fragmentos ausentes inicialmente é 1. 
        
        Isso simboliza que ainda é esperado o tamanho do arquivo.
    */
    novo_buffer->n_fragmentos_ausentes = 1;

    novo_buffer->tam_arquivo = 0;
    novo_buffer->dados_arquivo_obtidos = false;
    novo_buffer->arquivo_criado = false;

    inicializar_lista_mensagem(&novo_buffer->fragmentos_necessarios);

    novo_buffer->mutex_buffer = PTHREAD_MUTEX_INITIALIZER;

    return novo_buffer;
}

void trancar_buffer(buffer_t *buffer)
{
    pthread_mutex_lock(&buffer->mutex_buffer);
}

void destrancar_buffer(buffer_t *buffer)
{
    pthread_mutex_unlock(&buffer->mutex_buffer);
}

void informar_dados_arquivo(buffer_t *buffer, const unsigned tam_arquivo)
{
    buffer->tam_arquivo = tam_arquivo;
    buffer->dados_arquivo_obtidos = true;
}