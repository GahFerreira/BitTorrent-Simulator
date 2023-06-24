#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>

#include "lista_mensagem.h"

// `fragmento(buffer, i_fragmento)` acessa o [i]ésimo fragmento do buffer `*buffer`.
#define fragmento(buffer, i_fragmento) (buffer->dados_fragmentos[(i_fragmento * buffer->tam_fragmento)])

typedef unsigned char byte;
typedef struct buffer buffer_t;

struct buffer
{
    unsigned n_fragmentos, tam_fragmento;

    unsigned n_fragmentos_ausentes, id_primeiro_fragmento_atual;

    unsigned tam_arquivo;
    char nome_arquivo[256];
    bool dados_arquivo_obtidos, arquivo_criado;

    lista_mensagem_t fragmentos_necessarios;

    pthread_mutex_t mutex_buffer;
    
    // Os bytes que comporão os fragmentos.
    byte dados_fragmentos[];
};

buffer_t *construir_buffer(const unsigned n_fragmentos, const unsigned tam_fragmento);

void trancar_buffer(buffer_t *buffer);
void destrancar_buffer(buffer_t *buffer);

void informar_dados_arquivo(buffer_t *buffer, const unsigned tam_arquivo, const char nome_arquivo[]);

void atualizar_buffer(buffer_t *buffer);

#endif // BUFFER_H