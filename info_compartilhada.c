#include <stdlib.h>
#include <stdio.h>

#include "info_compartilhada.h"
#include "par_usuario_arquivo.h"

void construir_info_compartilhada(info_compartilhada_t *informacoes_compartilhadas, const unsigned n_usuarios, const unsigned n_arquivos)
{
    informacoes_compartilhadas->finalizar_execucao = false;
    
    informacoes_compartilhadas->n_usuarios_finalizados = 0;
    inicializar_dado_concorrente(&informacoes_compartilhadas->controlador_n_usuarios_finalizados, &informacoes_compartilhadas->n_usuarios_finalizados);

    informacoes_compartilhadas->n_usuarios = n_usuarios;
    informacoes_compartilhadas->n_arquivos = n_arquivos;

    // Calcula o chão de `log_10(n_usuarios) +1`.
    unsigned max_caracteres_dir_usuario = 0;
    unsigned aux = n_usuarios;
    while (aux > 0) 
    {
        ++max_caracteres_dir_usuario;
        aux /= 10;
    }
    // `+3` pois inclui os outros caracteres do diretório: "./U{numero_usuario}"
    informacoes_compartilhadas->max_caracteres_dir_usuario = max_caracteres_dir_usuario+3;

    // Inicializa a lista de mensagens de usuários conectados.
    inicializar_lista_mensagem(&informacoes_compartilhadas->usuarios_conectados);

    // Instancia e inicializa o vetor de listas de mensagens de conexão de novos usuários.
    inicializar_multiplas_listas_mensagem(&informacoes_compartilhadas->novos_usuarios_conectados, n_usuarios);

    // Instancia e inicializa o vetor de listas de solicitação de arquivos.
    inicializar_multiplas_listas_mensagem(&informacoes_compartilhadas->solicitacoes_arquivo, n_usuarios);

    // Instancia e inicializa o vetor de listas de conclusão de arquivos.
    inicializar_multiplas_listas_mensagem(&informacoes_compartilhadas->arquivo_completo, n_usuarios);
}

void enviar_solicitacao_arquivo(info_compartilhada_t *info_compartilhada, const unsigned usuario_fonte, const unsigned id_arquivo, const unsigned usuario_destino)
{
    // Tratamento de erros.
    if (usuario_fonte == usuario_destino)
    {
        printf("[[ERRO]] Usuario %u solicitou arquivo %u para si mesmo.[info_compartilhada::enviar_solicitacao_arquivo]\n\n", usuario_fonte+1, id_arquivo+1);

        return;
    }

    if (usuario_fonte >= info_compartilhada->n_usuarios)
    {
        printf("[[ERRO]] Solicitacao de arquivo por usuario %u, mas existem apenas %u usuarios.[info_compartilhada::enviar_solicitacao_arquivo]\n\n", usuario_fonte+1, info_compartilhada->n_usuarios);

        return;
    }

    if (usuario_destino >= info_compartilhada->n_usuarios)
    {
        printf("[[ERRO]] Solicitacao de arquivo enviada ao usuario %u, mas existem apenas %u usuarios.[info_compartilhada::enviar_solicitacao_arquivo]\n\n", usuario_destino+1, info_compartilhada->n_usuarios);

        return;
    }

    if (id_arquivo >= info_compartilhada->n_arquivos)
    {
        printf("[[ERRO]] Solicitacao de arquivo com id %u, mas existem apenas %u arquivos.[info_compartilhada::enviar_solicitacao_arquivo]\n\n", id_arquivo+1, info_compartilhada->n_arquivos);

        return;
    }
    // Fim do tratamento de erros.

    par_usuario_arquivo_t *requisicao = (par_usuario_arquivo_t *) calloc(1, sizeof(par_usuario_arquivo_t));
    requisicao->id_arquivo = id_arquivo;
    requisicao->id_usuario = usuario_fonte;

    adicionar_elemento_lista_mensagem(&info_compartilhada->solicitacoes_arquivo[usuario_destino], requisicao);
}