#include "processamento_mensagens.h"

void *processar_mensagens_recebidas(info_total_t *info_total)
{
    // Parte 1 - O usuário checa se há algum novo usuário conectado

    // Obtém o id do usuário atual.
    const unsigned id_usuario = info_total->info_usuario->id_usuario;

    // Obtém a lista de recebimento de novos usuários conectados do usuário atual.
    lista_mensagem_t *novos_usuarios_conectados = &info_total->info_compartilhada->novos_usuarios_conectados[id_usuario];

    while (info_total->info_compartilhada->finalizar_execucao == false)
    {
        // Primeiro passo - O usuário checa se há algum novo usuário conectado
        checar_novo_usuario_conectado(info_total, novos_usuarios_conectados, id_usuario);

        _sleep(2000);
    }

    return NULL;
}

#include "stdio.h"

void checar_novo_usuario_conectado(info_total_t *info_total, lista_mensagem_t *novos_usuarios_conectados, const unsigned id_usuario)
{
    #ifdef DEBUG
    printf("\nDEBUG: Usuario %u checa novos usuarios conectados.\n", id_usuario+1);
    #endif

    // Tenta extrair um dos novos usuários conectados.
    const unsigned *novo_usuario_conectado = (const unsigned *) extrair_primeiro_lista_mensagens(novos_usuarios_conectados);

    // Só realiza ação se houver um novo usuário conectado.
    if (novo_usuario_conectado != NULL)
    {
        #ifdef DEBUG 

        printf("\nDEBUG: Usuario %d encontra novo usuario conectado: usuario %d\n", id_usuario+1, *novo_usuario_conectado+1); 

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
        par_t *requisicoes[n_arquivos_em_progresso];

        for (unsigned i_requisicao = 0; i_requisicao < n_arquivos_em_progresso; ++i_requisicao)
        {
            /*
                Cada requisição é criada dinamicamente.
                É responsabilidade do consumidor da mensagem desalocá-la.
            */
            requisicoes[i_requisicao] = (par_t *) calloc(1, sizeof(par_t));

            requisicoes[i_requisicao]->usuario = id_usuario;
            requisicoes[i_requisicao]->id_arquivo = arquivos_em_progresso[i_requisicao];
        }

        adicionar_elementos_lista_mensagens(info_total->info_compartilhada->solicitacoes_arquivo, (const void **) requisicoes, n_arquivos_em_progresso);
    }
}