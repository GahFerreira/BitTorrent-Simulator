#include <stdlib.h>
#if DEBUG > 0
#include <stdio.h>
#endif

#include "solicitar_arquivos.h"
#include "util.h"

void *solicitar_arquivos(info_total_t *info_total)
{
    #if DEBUG >= 4
    printf("[DEBUG-3] Usuario %u iniciou solicitar_arquivos.\n\n", info_total->info_usuario->id_usuario+1);
    #endif

    // Obtém o id do usuário atual.
    const unsigned id_usuario = info_total->info_usuario->id_usuario;

    /*
        Obtém e mantém um vetor de booleanos `arquivos_ausentes`, tal que `arquivos_ausentes[x]`
        é `true` se e somente se o arquivo cujo id é 'x' estiver ausente do diretório do usuário. 
    */

    // LOCK
    trancar_info_arquivos(&info_total->info_usuario->info_arquivos);

    bool arquivos_ausentes[ info_total->info_usuario->info_arquivos.n_arquivos ];

    unsigned n_arquivos_ausentes = info_total->info_usuario->info_arquivos.n_vazios;
    const unsigned n_arquivos_total = info_total->info_usuario->info_arquivos.n_arquivos;
    
    // Novo escopo para desalocar `arquivos_inicialmente_ausentes` da pilha.
    {
        unsigned arquivos_inicialmente_ausentes[ n_arquivos_ausentes ];

        obter_arquivos_ausentes(&info_total->info_usuario->info_arquivos, arquivos_inicialmente_ausentes);

        destrancar_info_arquivos(&info_total->info_usuario->info_arquivos);
        // UNLOCK

        // Marca todos os arquivos ausentes em `arquivos_ausentes`.
        for (unsigned i_arq = 0, i_arq_inic_ausentes = 0; 
             i_arq < info_total->info_usuario->info_arquivos.n_arquivos; 
             ++i_arq)
        {
            if (i_arq == arquivos_inicialmente_ausentes[i_arq_inic_ausentes])
            {
                arquivos_ausentes[i_arq] = true;
                ++i_arq_inic_ausentes;
            }

            else arquivos_ausentes[i_arq] = false;
        }
    }

    /*
        A cada 3-10 segundos, o usuário vai fazer uma tentativa de solicitar arquivos.

        Dado que o usuário precisa de X arquivos, cada arquivo tem (100/X)% de chance de ser solicitado
        durante essa tentativa.

        Isso dá cerca de 1 arquivo por tentativa.
    */
    while (info_total->info_compartilhada->finalizar_execucao == false && n_arquivos_ausentes > 0)
    {
        #if DEBUG >= 5
        printf("[DEBUG-1] Usuario %u tenta solicitar arquivos.\n\n", id_usuario+1);
        #endif

        const unsigned chance = 100 / n_arquivos_ausentes;

        for (unsigned i_arquivo = 0; i_arquivo < n_arquivos_total; ++i_arquivo)
        {
            if (arquivos_ausentes[i_arquivo] == true)
            {
                if (aleatorio(1, 100) <= chance) 
                {
                    arquivos_ausentes[i_arquivo] = false;

                    solicitar_arquivo(info_total->info_compartilhada, &info_total->info_usuario->info_arquivos, id_usuario, i_arquivo);

                    #if DEBUG >= 2
                    printf("[DEBUG-1] Usuario %u solicita arquivo %u.\n\n", id_usuario+1, i_arquivo+1);
                    #endif
                }
            }
            
        }

        meu_sleep(3000 + aleatorio(0, 7000));
    }

    pthread_exit(NULL);

    return NULL; // Sem retorno.
}

void solicitar_arquivo(info_compartilhada_t *info_compartilhada, info_arquivos_t *info_arquivos, const unsigned id_usuario, const unsigned id_arquivo)
{
    mudar_arquivo_para_em_progresso(info_arquivos, id_usuario, id_arquivo);

    for (unsigned i_usuario = 0; i_usuario < info_compartilhada->n_usuarios; ++i_usuario)
    {
        if (i_usuario == id_usuario) continue;

        enviar_solicitacao_arquivo(info_compartilhada, id_usuario, id_arquivo, i_usuario);
    }
}