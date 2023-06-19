#include <stdlib.h>

#include "info_compartilhada.h"

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