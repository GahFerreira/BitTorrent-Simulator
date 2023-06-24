/**
 * Estrutura de dados pública com propósito de ser acessada 
 * por todos os usuários durante a execução do programa.
*/

#ifndef INFO_COMPARTILHADA_H
#define INFO_COMPARTILHADA_H

#include <stdbool.h>

#include "lista_mensagem.h"
#include "buffer.h"

typedef struct info_compartilhada info_compartilhada_t;

struct info_compartilhada
{
    // Estados de execução.
    bool finalizar_execucao;
    
    unsigned n_usuarios_finalizados;
    pthread_mutex_t mutex_n_usuarios_finalizados;

    unsigned id_prox_usuario;
    pthread_mutex_t mutex_id_prox_usuario;

    unsigned n_usuarios;
    unsigned n_arquivos;
    unsigned n_fragmentos_buffer;
    unsigned tam_fragmento;

    // Usado para instanciar nomes de diretórios de usuários corretamente.
    unsigned max_caracteres_dir_usuario;

    // Lista os usuários conectados.
    lista_mensagem_t usuarios_conectados;

    // `novos_usuarios_conectados[x]` contém um inteiro y.
    // Isso diz ao usuario 'x' que o usuário 'y' se conectou.
    lista_mensagem_t *novos_usuarios_conectados;
    // `solicitacoes_arquivo[x]` conterá um par `<y, arq_id>.`
    // Isso diz que o usuário 'y' pede ao usuário 'x' pelo arquivo cujo id é `arq_id`.
    lista_mensagem_t *solicitacoes_arquivo;
    // `arquivo_completo[x]` conterá um par `<y, arq_id>.`
    // Isso diz que o usuário 'y' está contando ao usuário 'x' que agora 'y' possui
    // o arquivo cujo id é `arq_id` disponível.
    lista_mensagem_t *arquivo_completo;

    /*
        Buffers usados para transferência de arquivos.

        buffers_usuarios[x] (que é buffer_t **) representa a lista de buffers do usuário 'x'.
        buffers_usuarios[x][y] (que é buffer_t *) é um ponteiro para o buffer 'y' do usuário 'x' (inicialmente NULL).
        *buffers_usuarios[x][y] (que é buffer_t) é o [y]-ésimo buffer do usuário 'x'.

        Idealmente seria um ponteiro duplo, mas como o buffer tem que ser alocado com `malloc`,
        preciso de um nível a mais de deferência.
    */
    buffer_t ***buffers_usuarios;
};

void inicializar_info_compartilhada(info_compartilhada_t *info_compartilhada, const unsigned n_usuarios, const unsigned n_arquivos, const unsigned n_fragmentos_buffer, const unsigned tam_fragmento);

unsigned obter_id_prox_usuario(info_compartilhada_t *info_compartilhada);

/*
    O `usuario_fonte` envia uma mensagem ao `usuario_destino` pedindo o arquivo cujo id é `id_arquivo`.

    Nota: aloca memória, que deverá ser liberada ao processar a mensagem.
*/
void enviar_solicitacao_arquivo(info_compartilhada_t *info_compartilhada, const unsigned usuario_fonte, const unsigned id_arquivo, const unsigned usuario_destino);

bool criar_buffer(info_compartilhada_t *info_compartilhada, const unsigned id_usuario, const unsigned id_arquivo);
bool finalizar_buffer(info_compartilhada_t *info_compartilhada, const unsigned id_usuario, const unsigned id_arquivo);

#endif // INFO_COMPARTILHADA_H
