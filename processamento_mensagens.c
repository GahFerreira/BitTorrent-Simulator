#ifdef DEBUG
#include <stdio.h>
#endif

#include "processamento_mensagens.h"
#include "par_usuario_arquivo.h"
#include "util.h"

void *processar_mensagens_recebidas(info_total_t *info_total)
{
    // Parte 1 - O usuário checa se há algum novo usuário conectado

    // Obtém o id do usuário atual.
    const unsigned id_usuario = info_total->info_usuario->id_usuario;

    // Obtém a lista de recebimento de novos usuários conectados do usuário atual.
    lista_mensagem_t *novos_usuarios_conectados = &info_total->info_compartilhada->novos_usuarios_conectados[id_usuario];

    // Obtém a lista de recebimento de mensagens de completude de arquivos.
    lista_mensagem_t *arquivo_completo = &info_total->info_compartilhada->arquivo_completo[id_usuario];

    // Obtém a lista de solicitações de arquivos.
    lista_mensagem_t *solicitacoes_arquivo = &info_total->info_compartilhada->solicitacoes_arquivo[id_usuario];

    while (info_total->info_compartilhada->finalizar_execucao == false)
    {
        // Primeiro passo - O usuário checa se há algum novo usuário conectado.
        checar_novo_usuario_conectado(info_total, novos_usuarios_conectados, id_usuario);

        // Segundo passo - O usuário checa se há alguma nova mensagem de completude de arquivo.
        checar_mensagem_arquivo_completo(info_total, arquivo_completo, id_usuario);

        // Terceiro passo - O usuário checa se há alguma nova solicitação de arquivo.
        checar_solicitacoes_arquivo(info_total, solicitacoes_arquivo, id_usuario);

        meu_sleep((id_usuario+1) * 750);
    }

    return NULL;
}

void checar_novo_usuario_conectado(info_total_t *info_total, lista_mensagem_t *novos_usuarios_conectados, const unsigned id_usuario)
{
    #ifdef DEBUG
    printf("\nDEBUG: Usuario %u checa novos usuarios conectados.\n", id_usuario+1);
    #endif

    // Tenta extrair um dos novos usuários conectados.
    const unsigned *novo_usuario_conectado = (const unsigned *) extrair_primeiro_lista_mensagem(novos_usuarios_conectados);

    // Só realiza ação se houver um novo usuário conectado.
    if (novo_usuario_conectado != NULL)
    {
        #ifdef DEBUG 
        printf("\nDEBUG: Usuario %u encontra novo usuario conectado: usuario %u\n", id_usuario+1, *novo_usuario_conectado+1); 
        #endif

        unsigned n_arquivos_em_progresso;

        /*
            Precisa obter o mutex de `info_arquivos`, pois realiza dois acessos:
            1 - Recupera o número de arquivos em progresso
            2 - Recupera os arquivos em progresso
        */
        abrir_info_arquivos(&info_total->info_usuario->controlador_info_arquivos);

        /*
            Cria um vetor para guardar os arquivos que ainda estão em progresso.
            Esse vetor é exatamente do tamanho necessário.
        */
        n_arquivos_em_progresso = ((info_arquivos_t *) (info_total->info_usuario->controlador_info_arquivos.dado))->n_em_progresso;

        unsigned arquivos_em_progresso[n_arquivos_em_progresso];

        obter_arquivos_em_progresso(&info_total->info_usuario->controlador_info_arquivos, arquivos_em_progresso);

        fechar_info_arquivos(&info_total->info_usuario->controlador_info_arquivos);

        /*
            Envia ao novo usuário conectado os arquivos que precisa.
        */
        par_usuario_arquivo_t *requisicoes[n_arquivos_em_progresso];

        for (unsigned i_requisicao = 0; i_requisicao < n_arquivos_em_progresso; ++i_requisicao)
        {
            /*
                Cada requisição é criada dinamicamente.
                É responsabilidade do consumidor da mensagem desalocá-la.
            */
            requisicoes[i_requisicao] = (par_usuario_arquivo_t *) calloc(1, sizeof(par_usuario_arquivo_t));

            requisicoes[i_requisicao]->usuario = id_usuario;
            requisicoes[i_requisicao]->id_arquivo = arquivos_em_progresso[i_requisicao];
        }

        adicionar_elementos_lista_mensagem(info_total->info_compartilhada->solicitacoes_arquivo, (const void **) requisicoes, n_arquivos_em_progresso);
    }
}

void checar_mensagem_arquivo_completo(info_total_t *info_total, lista_mensagem_t *arquivo_completo, const unsigned id_usuario)
{
    #ifdef DEBUG
    printf("\nDEBUG: Usuario %u checa novas mensagens de completude de arquivos.\n", id_usuario+1);
    #endif

    // Tenta extrair uma nova mensagem de arquivo completo.
    const par_usuario_arquivo_t *mensagem_arquivo_completo = (const par_usuario_arquivo_t *) extrair_primeiro_lista_mensagem(arquivo_completo);

    if (mensagem_arquivo_completo != NULL)
    {
        #ifdef DEBUG 
        printf("\nDEBUG: Usuario %u encontra nova mensagem de arquivo completo: <usuario: %u, arquivo: %u>\n", id_usuario+1, mensagem_arquivo_completo->usuario, mensagem_arquivo_completo->id_arquivo); 
        #endif

        // Obtém o estado do arquivo dito como completo.
        estado_progresso_t estado_arquivo = obter_estado_arquivo(&info_total->info_usuario->controlador_info_arquivos, mensagem_arquivo_completo->id_arquivo);

        // Envia uma solicitação de arquivo ao usuário da mensagem.
        if (estado_arquivo == EM_PROGRESSO)
        {
            enviar_solicitacao_arquivo(info_total->info_compartilhada, id_usuario, mensagem_arquivo_completo->id_arquivo, mensagem_arquivo_completo->usuario);
        }

        // Retira <usuário_mensagem, arquivo_mensagem> da sua lista de tarefas.
        else if (estado_arquivo == COMPLETO)
        {
            completar_tarefa(&info_total->info_usuario->lista_tarefas, mensagem_arquivo_completo->usuario, mensagem_arquivo_completo->id_arquivo);
        }

        // TODO: Talvez colocar numa função à parte?
        free((par_usuario_arquivo_t *) mensagem_arquivo_completo);
    }
}

void checar_solicitacoes_arquivo(info_total_t *info_total, lista_mensagem_t *solicitacoes_arquivo, const unsigned id_usuario)
{
    #ifdef DEBUG
    printf("\nDEBUG: Usuario %u checa solicitacoes de arquivo.\n", id_usuario+1);
    #endif

    const par_usuario_arquivo_t *solicitacao_arquivo = (const par_usuario_arquivo_t *) extrair_primeiro_lista_mensagem(solicitacoes_arquivo);

    if (solicitacao_arquivo != NULL)
    {
        #ifdef DEBUG 
        printf("\nDEBUG: Usuario %u encontra nova solicitacao de arquivo: <usuario: %u, arquivo: %u>\n", id_usuario+1, solicitacao_arquivo->usuario, solicitacao_arquivo->id_arquivo); 
        #endif

        estado_progresso_t estado_arquivo = obter_estado_arquivo(&info_total->info_usuario->controlador_info_arquivos, solicitacao_arquivo->id_arquivo);

        if (estado_arquivo == COMPLETO)
        {
            adicionar_tarefa(&info_total->info_usuario->lista_tarefas, id_usuario, solicitacao_arquivo->id_arquivo);
        }
    }
}