#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>

#include "lista_mensagem.h"

#define INVALIDO UINT_MAX

// `acessar_fragmento(buffer, i_fragmento)` acessa o [i]ésimo fragmento do buffer `*buffer`.
#define acessar_fragmento(buffer, i_fragmento) (buffer->dados_fragmentos[(unsigned) (i_fragmento * buffer->tam_fragmento)])

typedef unsigned char byte;
typedef struct buffer buffer_t;

struct buffer
{
    unsigned n_fragmentos, tam_fragmento;

    unsigned n_fragmentos_ausentes, id_primeiro_fragmento_atual;

    unsigned tam_arquivo;
    char nome_arquivo[256];

    bool dados_arquivo_obtidos, arquivo_criado, primeira_execucao;

    lista_encadeada_t fragmentos_necessarios;

    pthread_mutex_t mutex_buffer;
    
    // Os bytes que comporão os fragmentos.
    byte dados_fragmentos[];
};

buffer_t *construir_buffer(const unsigned n_fragmentos, const unsigned tam_fragmento);
void destruir_buffer(buffer_t *buffer);

void trancar_buffer(buffer_t *buffer);
bool tentar_trancar_buffer(buffer_t *buffer);
void destrancar_buffer(buffer_t *buffer);

void informar_dados_arquivo(buffer_t *buffer, const unsigned tam_arquivo, const char nome_arquivo[]);

// Retorna `true` se o arquivo tiver sido completamente gravado.
bool atualizar_buffer(buffer_t *buffer);
void adicionar_id_fragmento_necessario_na_lista(buffer_t *buffer, const unsigned id_fragmento);
unsigned obter_id_fragmento_necessario(buffer_t *buffer);
void gravar_fragmento_buffer(buffer_t *buffer, const unsigned id_fragmento, byte fragmento[]);
unsigned obter_quantidade_bytes_para_gravar(buffer_t *buffer);
unsigned obter_quantidade_bytes_para_ler(buffer_t *buffer, const unsigned id_fragmento);

#endif // BUFFER_H