#include <stdlib.h>

#include "info_compartilhada.h"

void construir_info_compartilhada(info_compartilhada_t *informacoes_compartilhadas, const unsigned n_usuarios, const unsigned n_arquivos)
{
    informacoes_compartilhadas->n_usuarios = n_usuarios;
    informacoes_compartilhadas->n_arquivos = n_arquivos;

    // Calcula o chão de `log_10(n_usuarios) +1`.
    unsigned max_caracteres_dir_usuario = 0;
    unsigned aux = n_usuarios;
    while (aux > 0) 
    {
        max_caracteres_dir_usuario++;
        aux /= 10;
    }
    // `+3` pois inclui os outros caracteres do diretório: "./U{numero_usuario}"
    informacoes_compartilhadas->max_caracteres_dir_usuario = max_caracteres_dir_usuario+3;

    // Inicia o mapa de `id_arquivo` para `nome_arquivo`.
    informacoes_compartilhadas->id_arquivo_para_nome = (char **) calloc(n_arquivos, sizeof(char *));
    for (unsigned i_arquivo = 0; i_arquivo < n_arquivos; i_arquivo++)
    {
        informacoes_compartilhadas->id_arquivo_para_nome[i_arquivo] = NULL;
    }

    // Instancia e inicializa o vetor de listas de solicitação de arquivos.
    informacoes_compartilhadas->solicitacoes_arquivo = (lista_mensagem_t *) calloc(n_usuarios, sizeof(lista_mensagem_t));
    for (unsigned i_usuario = 0; i_usuario < n_usuarios; i_usuario++)
    {
        inicializar_lista_mensagem(&informacoes_compartilhadas->solicitacoes_arquivo[i_usuario]);
    }

    // Instancia e inicializa o vetor de listas de conclusão de arquivos.
    informacoes_compartilhadas->arquivo_completo = (lista_mensagem_t *) calloc(n_usuarios, sizeof(lista_mensagem_t));
    for (unsigned i_usuario = 0; i_usuario < n_usuarios; i_usuario++)
    {
        inicializar_lista_mensagem(&informacoes_compartilhadas->arquivo_completo[i_usuario]);
    }
}