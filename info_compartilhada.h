/**
 * Estrutura de dados pública com propósito de ser acessada 
 * por todos os usuários durante a execução do programa.
*/

#ifndef INFO_COMPARTILHADA_H
#define INFO_COMPARTILHADA_H

#include <stdbool.h>

#include "dado_concorrente.h"
#include "lista_mensagem.h"

typedef struct info_compartilhada info_compartilhada_t;
typedef struct par par_t;

struct info_compartilhada
{
    // Estados de execução.
    bool finalizar_execucao;
    unsigned n_usuarios_finalizados;
    dado_concorrente_t controlador_n_usuarios_finalizados;

    unsigned n_usuarios;
    unsigned n_arquivos;
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
};

struct par
{
    unsigned usuario, id_arquivo;
};

void construir_info_compartilhada(info_compartilhada_t *informacoes_compartilhadas, const unsigned n_usuarios, const unsigned n_arquivos);

#endif // INFO_COMPARTILHADA_H
