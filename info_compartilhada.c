#include <stdlib.h>
#include <stdio.h>

#include "info_compartilhada.h"
#include "par_usuario_arquivo.h"

void inicializar_info_compartilhada(info_compartilhada_t *info_compartilhada, const unsigned n_usuarios, const unsigned n_arquivos, const unsigned n_fragmentos_buffer, const unsigned tam_fragmento)
{
    info_compartilhada->finalizar_execucao = false;
    
    info_compartilhada->n_usuarios_finalizados = 0;
    pthread_mutex_init(&info_compartilhada->mutex_n_usuarios_finalizados, NULL);

    info_compartilhada->id_prox_usuario = 0;
    pthread_mutex_init(&info_compartilhada->mutex_id_prox_usuario, NULL);

    info_compartilhada->n_usuarios = n_usuarios;
    info_compartilhada->n_arquivos = n_arquivos;
    info_compartilhada->n_fragmentos_buffer = n_fragmentos_buffer;
    info_compartilhada->tam_fragmento = tam_fragmento;

    // Calcula o chão de `log_10(n_usuarios) +1`.
    unsigned max_caracteres_dir_usuario = 0;
    unsigned aux = n_usuarios;
    while (aux > 0) 
    {
        ++max_caracteres_dir_usuario;
        aux /= 10;
    }
    // `+4` pois inclui os outros caracteres do diretório: "./U{numero_usuario}" e um caractere de fim.
    info_compartilhada->max_caracteres_dir_usuario = max_caracteres_dir_usuario+3;

    // Inicializa a lista de mensagens de usuários conectados.
    inicializar_lista_mensagem(&info_compartilhada->usuarios_conectados);

    // Instancia e inicializa o vetor de listas de mensagens de conexão de novos usuários.
    inicializar_multiplas_listas_mensagem(&info_compartilhada->novos_usuarios_conectados, n_usuarios);

    // Instancia e inicializa o vetor de listas de solicitação de arquivos.
    inicializar_multiplas_listas_mensagem(&info_compartilhada->solicitacoes_arquivo, n_usuarios);

    // Instancia e inicializa o vetor de listas de conclusão de arquivos.
    inicializar_multiplas_listas_mensagem(&info_compartilhada->arquivo_completo, n_usuarios);

    info_compartilhada->buffers_usuarios = (buffer_t ***) malloc(n_usuarios * sizeof(buffer_t **));
    for (unsigned i_usuario = 0; i_usuario < n_usuarios; ++i_usuario)
    {
        info_compartilhada->buffers_usuarios[i_usuario] = (buffer_t **) malloc(n_arquivos * sizeof(buffer_t *));

        for (unsigned i_arquivo = 0; i_arquivo < n_arquivos; ++i_arquivo)
        {
            // `malloc` nem `calloc` aloca os ponteiros com `NULL`.
            info_compartilhada->buffers_usuarios[i_usuario][i_arquivo] = NULL;
        }
    }
}

unsigned obter_id_prox_usuario(info_compartilhada_t *info_compartilhada)
{
    unsigned resultado;

    pthread_mutex_lock(&info_compartilhada->mutex_id_prox_usuario);

    resultado = info_compartilhada->id_prox_usuario;

    ++info_compartilhada->id_prox_usuario;

    pthread_mutex_unlock(&info_compartilhada->mutex_id_prox_usuario);

    return resultado;
}

void novo_usuario_finalizado(info_compartilhada_t *info_compartilhada)
{
    pthread_mutex_lock(&info_compartilhada->mutex_n_usuarios_finalizados);

    --info_compartilhada->n_usuarios_finalizados;

    pthread_mutex_unlock(&info_compartilhada->mutex_n_usuarios_finalizados);

    if (info_compartilhada->n_usuarios_finalizados == info_compartilhada->n_usuarios)
    {
        info_compartilhada->finalizar_execucao = true;
    }
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

void enviar_mensagem_arquivo_completo(info_compartilhada_t *info_compartilhada, const unsigned usuario_fonte, const unsigned id_arquivo, const unsigned usuario_destino)
{
    // Tratamento de erros.
    if (usuario_fonte == usuario_destino)
    {
        printf("[[ERRO]] Usuario %u enviou mensagem do arquivo %u completo para si mesmo.[info_compartilhada::enviar_mensagem_arquivo_completo]\n\n", usuario_fonte+1, id_arquivo+1);

        return;
    }

    if (usuario_fonte >= info_compartilhada->n_usuarios)
    {
        printf("[[ERRO]] Mensagem de arquivo completo pelo usuario %u, mas existem apenas %u usuarios.[info_compartilhada::enviar_mensagem_arquivo_completo]\n\n", usuario_fonte+1, info_compartilhada->n_usuarios);

        return;
    }

    if (usuario_destino >= info_compartilhada->n_usuarios)
    {
        printf("[[ERRO]] Mensagem de arquivo completo enviada ao usuario %u, mas existem apenas %u usuarios.[info_compartilhada::enviar_mensagem_arquivo_completo]\n\n", usuario_destino+1, info_compartilhada->n_usuarios);

        return;
    }

    if (id_arquivo >= info_compartilhada->n_arquivos)
    {
        printf("[[ERRO]] Mensagem de arquivo completo com id %u, mas existem apenas %u arquivos.[info_compartilhada::enviar_mensagem_arquivo_completo]\n\n", id_arquivo+1, info_compartilhada->n_arquivos);

        return;
    }
    // Fim do tratamento de erros.

    par_usuario_arquivo_t *mensagem = (par_usuario_arquivo_t *) calloc(1, sizeof(par_usuario_arquivo_t));
    mensagem->id_arquivo = id_arquivo;
    mensagem->id_usuario = usuario_fonte;

    adicionar_elemento_lista_mensagem(&info_compartilhada->solicitacoes_arquivo[usuario_destino], mensagem);
}

bool criar_buffer(info_compartilhada_t *info_compartilhada, const unsigned id_usuario, const unsigned id_arquivo)
{
    // Tratamento de erros.
    if (id_usuario >= info_compartilhada->n_usuarios)
    {
        printf("[[ERRO]] Tentativa de criar buffer para o arquivo %u do usuario %u, mas so ha %u usuarios. [info_compartilhada::criar_buffer]\n\n", id_arquivo+1, id_usuario+1, info_compartilhada->n_usuarios);

        return false;
    }

    if (id_arquivo >= info_compartilhada->n_arquivos)
    {
        printf("[[ERRO]] Tentativa de criar buffer para o arquivo %u do usuario %u, mas so ha %u arquivos. [info_compartilhada::criar_buffer]\n\n", id_arquivo+1, id_usuario+1, info_compartilhada->n_arquivos);

        return false;
    }

    /*
        Buffer que receberá o arquivo.
        
        TODO: Alias não foi usado porque ocorria um bug que precisa ser melhor checado. Talvez usar &?
    */
    if (info_compartilhada->buffers_usuarios[id_usuario][id_arquivo] != NULL)
    {
        printf("[[ERRO]] Falha em criar buffer para o arquivo %u do usuario %u: o buffer especificado ja existe ou esta corrompido. [info_compartilhada::criar_buffer]\n\n", id_arquivo+1, id_usuario+1);

        return false;
    }

    #if DEBUG >= 6
    printf("[DEBUG-6] Construcao de buffer para receber arquivo %u do usuario %u.\n\n", id_arquivo+1, id_usuario+1);
    #endif

    info_compartilhada->buffers_usuarios[id_usuario][id_arquivo] = construir_buffer(info_compartilhada->n_fragmentos_buffer, info_compartilhada->tam_fragmento);

    return true;
}

bool finalizar_buffer(info_compartilhada_t *info_compartilhada, const unsigned id_usuario, const unsigned id_arquivo)
{
    // Tratamento de erros.
    if (id_usuario >= info_compartilhada->n_usuarios)
    {
        printf("[[ERRO]] Tentativa de finalizar buffer do arquivo %u do usuario %u, mas so ha %u usuarios. [info_compartilhada::finalizar_buffer]\n\n", id_arquivo+1, id_usuario+1, info_compartilhada->n_usuarios);

        return false;
    }

    if (id_arquivo >= info_compartilhada->n_arquivos)
    {
        printf("[[ERRO]] Tentativa de finalizar buffer para o arquivo %u pelo usuario %u, mas so ha %u arquivos. [info_compartilhada::finalizar_buffer]\n\n", id_arquivo+1, id_usuario+1, info_compartilhada->n_arquivos);

        return false;
    }

    // Recebe o endereço no qual o buffer deverá ser finalizado.
    buffer_t *buffer_arquivo = info_compartilhada->buffers_usuarios[id_arquivo][id_arquivo];

    if (buffer_arquivo != NULL)
    {
        printf("[[ERRO]] Falha em finalizar buffer para o arquivo %u pelo usuario %u: o buffer especificado nao existe. [info_compartilhada::finalizar_buffer]\n\n", id_arquivo+1, id_usuario+1);

        return false;
    }

    destruir_buffer(buffer_arquivo);

    info_compartilhada->buffers_usuarios[id_arquivo][id_arquivo] = NULL;

    return true;
}