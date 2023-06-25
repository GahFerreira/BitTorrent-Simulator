#include <stdlib.h>
#include <string.h>

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
    novo_buffer->id_primeiro_fragmento_atual = 0;

    novo_buffer->tam_arquivo = 0;
    novo_buffer->nome_arquivo[0] = '\0';

    novo_buffer->dados_arquivo_obtidos = false;
    novo_buffer->arquivo_criado = false;
    novo_buffer->primeira_execucao = true;

    inicializar_lista_encadeada(&novo_buffer->fragmentos_necessarios);

    novo_buffer->mutex_buffer = PTHREAD_MUTEX_INITIALIZER;

    return novo_buffer;
}

void destruir_buffer(buffer_t *buffer)
{
    free(buffer);
}

void trancar_buffer(buffer_t *buffer)
{
    pthread_mutex_lock(&buffer->mutex_buffer);
}

bool tentar_trancar_buffer(buffer_t *buffer)
{
    if (pthread_mutex_trylock(&buffer->mutex_buffer) == 0) return true;
    return false;
}

void destrancar_buffer(buffer_t *buffer)
{
    pthread_mutex_unlock(&buffer->mutex_buffer);
}

void informar_dados_arquivo(buffer_t *buffer, const unsigned tam_arquivo, const char nome_arquivo[])
{
    buffer->tam_arquivo = tam_arquivo;
    strcpy(buffer->nome_arquivo, nome_arquivo);

    buffer->dados_arquivo_obtidos = true;
}

bool atualizar_buffer(buffer_t *buffer)
{
    if (buffer->primeira_execucao == true)
    {
        buffer->id_primeiro_fragmento_atual = 0;

        buffer->primeira_execucao = false;
    }

    else buffer->id_primeiro_fragmento_atual += buffer->n_fragmentos;

    /*
        Atualiza o número de fragmentos ausentes no buffer. 
        Importante para os fragmentos finais, que podem não encher completamente o buffer.
    */
    const unsigned n_bytes_ja_gravados = buffer->id_primeiro_fragmento_atual * buffer->tam_fragmento;

    if (n_bytes_ja_gravados >= buffer->tam_arquivo) return true;

    const unsigned n_bytes_restantes_para_gravar = buffer->tam_arquivo - n_bytes_ja_gravados;

    buffer->n_fragmentos_ausentes = n_bytes_restantes_para_gravar / buffer->tam_fragmento;

    if (buffer->n_fragmentos_ausentes > buffer->n_fragmentos)
    {
        buffer->n_fragmentos_ausentes = buffer->n_fragmentos;
    }

    for (unsigned i_fragmento_ausente = 0; i_fragmento_ausente < buffer->n_fragmentos_ausentes; ++i_fragmento_ausente)
    {
        adicionar_id_fragmento_necessario_na_lista(buffer, buffer->id_primeiro_fragmento_atual + i_fragmento_ausente);
    }

    return false;
}

void adicionar_id_fragmento_necessario_na_lista(buffer_t *buffer, const unsigned id_fragmento)
{
    unsigned *novo_fragmento = malloc(sizeof(unsigned));
    *novo_fragmento = id_fragmento;
    adicionar_elemento_lista_encadeada(&buffer->fragmentos_necessarios, novo_fragmento);
}

unsigned obter_id_fragmento_necessario(buffer_t *buffer)
{
    if (esta_vazia_lista_encadeada(&buffer->fragmentos_necessarios) == true)
    {
        return INVALIDO;
    }

    const unsigned *id_fragmento_necessario = extrair_primeiro_lista_encadeada(&buffer->fragmentos_necessarios);
    unsigned resposta = *id_fragmento_necessario;

    free((unsigned *) id_fragmento_necessario);

    return resposta;
}

// Grava um fragmento no buffer.
void gravar_fragmento_buffer(buffer_t *buffer, const unsigned id_fragmento, byte fragmento[])
{
    memcpy(&acessar_fragmento(buffer, id_fragmento), fragmento, sizeof(byte) * buffer->tam_fragmento);

    --buffer->n_fragmentos_ausentes;
}

unsigned obter_quantidade_bytes_para_gravar(buffer_t *buffer)
{
    const unsigned n_bytes_ja_gravados = buffer->id_primeiro_fragmento_atual * buffer->tam_fragmento;
    const unsigned n_bytes_restantes_para_gravar = buffer->tam_arquivo - n_bytes_ja_gravados;
    const unsigned tamanho_buffer_cheio = buffer->n_fragmentos * buffer->tam_fragmento;

    if (n_bytes_restantes_para_gravar < tamanho_buffer_cheio) return n_bytes_restantes_para_gravar;
    return tamanho_buffer_cheio;
}

unsigned obter_quantidade_bytes_para_ler(buffer_t *buffer, const unsigned id_fragmento)
{
    /*
        Se ler todos os fragmentos antes desse + esse passa o tamanho do arquivo,
        então eu leio 
    */
    if ((id_fragmento+1) * buffer->tam_fragmento > buffer->tam_arquivo)
    {
        return buffer->tam_arquivo - (id_fragmento * buffer->tam_fragmento);
    }

    return buffer->tam_fragmento;
}