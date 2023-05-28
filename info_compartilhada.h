/**
 * Estrutura de dados pública com propósito de ser acessada por todas as threads 
 * durante a execução do programa.
*/

#ifndef INFO_COMPARTILHADA_H
#define INFO_COMPARTILHADA_H

#include "lista_mensagem.h"

typedef struct info_compartilhada info_compartilhada_t;

struct info_compartilhada
{
    unsigned n_usuarios;
    unsigned n_arquivos;
    // Usado para instanciar nomes de diretórios de usuários corretamente.
    unsigned max_caracteres_dir_usuario;

    // Útil para que todos os usuários obtenham todos os arquivos.
    // `id_arquivo_para_nome[x]` possui o nome do arquivo cujo id é 'x'.
    // Nota: apesar do nome dos arquivos ser 1-based, a representação
    // nesta estrutura é 0-based (o 1º arquivo tem índice 0).
    char **id_arquivo_para_nome;

    // Lista os usuários conectados.
    lista_mensagem_t usuarios_conectados;

    // `novo_usuario_conectado[x]` contém um inteiro y.
    // Isso diz ao usuario 'x' que o usuário 'y' se conectou.
    lista_mensagem_t *novo_usuario_conectado;
    // `solicitacoes_arquivo[x]` conterá um par `<y, arq_id>.`
    // Isso diz que o usuário 'y' pede ao usuário 'x' pelo arquivo cujo id é `arq_id`.
    lista_mensagem_t *solicitacoes_arquivo;
    // `arquivo_completo[x]` conterá um par `<y, arq_id>.`
    // Isso diz que o usuário 'y' está contando ao usuário 'x' que agora 'y' possui
    // o arquivo cujo id é `arq_id` disponível.
    lista_mensagem_t *arquivo_completo;
};

void construir_info_compartilhada(info_compartilhada_t *informacoes_compartilhadas, const unsigned n_usuarios, const unsigned n_arquivos);

#endif // INFO_COMPARTILHADA_H
