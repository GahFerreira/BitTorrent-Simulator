#include "enviar_fragmentos.h"
#include "buffer.h"
#include "util.h"

void *enviar_fragmentos(info_total_t *info_total)
{
    const unsigned id_usuario = info_total->info_usuario->id_usuario;

    par_usuario_arquivo_t tarefa;

    while (info_total->info_compartilhada->finalizar_execucao == false)
    {
        #if DEBUG >= 8
        printf("[DEBUG-8] Usuario %u tenta obter proxima tarefa.\n\n", id_usuario+1);
        #endif

        if (obter_proxima_tarefa(&info_total->info_usuario->lista_tarefas, &tarefa) == true)
        {
            #if DEBUG >= 7
            printf("[DEBUG-7] Tentativa de acessar buffer do usuario %u arquivo %u.\n\n", tarefa.id_usuario+1, tarefa.id_arquivo+1);
            #endif

            buffer_t *buffer_usuario_arquivo = info_total->info_compartilhada->buffers_usuarios[tarefa.id_usuario][tarefa.id_arquivo];

            // Acessa e tranca o buffer do usuário se ele não estiver trancado.
            if ( tentar_trancar_buffer(buffer_usuario_arquivo) == true )
            {
                // Obtém o id de um fragmento do arquivo da tarefa.
                const unsigned id_fragmento = obter_id_fragmento_necessario(buffer_usuario_arquivo);

                if (id_fragmento == INVALIDO)
                {
                    destrancar_buffer(buffer_usuario_arquivo);
                    continue;
                }

                const unsigned n_bytes_para_ler = obter_quantidade_bytes_para_ler(buffer_usuario_arquivo, id_fragmento);

                #if DEBUG >= 4
                if (n_bytes_para_ler != buffer_usuario_arquivo->tam_fragmento)
                {
                    printf("[DEBUG-4] Requisicao de leitura de fragmento de %u bytes do arquivo %u do usuario %u para o usuario %u.\n\n", n_bytes_para_ler, tarefa.id_arquivo+1, id_usuario+1, tarefa.id_usuario+1);
                }
                #endif

                destrancar_buffer(buffer_usuario_arquivo);

                byte fragmento[n_bytes_para_ler];

                // Lê o fragmento do arquivo no disco.
                if ( obter_fragmento_disco(info_total->info_usuario->manipulador_arquivos.ponteiro_arquivos[tarefa.id_arquivo], id_fragmento, info_total->info_compartilhada->tam_fragmento, n_bytes_para_ler, fragmento) == false )
                {
                    printf("[[ERRO]] Falha na leitura do disco do fragmento %u do arquivo %u do usuario %u. [enviar_fragmentos::enviar_fragmentos]\n\n", id_fragmento, tarefa.id_arquivo+1, id_usuario+1);
                }

                trancar_buffer(buffer_usuario_arquivo);

                gravar_fragmento_buffer(buffer_usuario_arquivo, id_fragmento, fragmento, n_bytes_para_ler);

                destrancar_buffer(buffer_usuario_arquivo);
            }
        }
    }

    pthread_exit(NULL);

    return NULL;
}

/*
    Lê `n_bytes_leitura` bytes do arquivo na posição determinada por `id_fragmento` e salva em `destino`.
    Supõe que:
      - Destino tenha o tamanho adequado.
      - `id_fragmento` seja um fragmento válido pro arquivo.
      - A leitura de todos os fragmentos exceto o último será feita passando `n_bytes_leitura` == `tam_fragmento`.
      - A leitura do último fragmento será realizada passando o `n_bytes_leitura` adequado.
*/
bool obter_fragmento_disco(FILE *arquivo_fonte, const unsigned id_fragmento, const unsigned tam_fragmento, const unsigned n_bytes_leitura, byte destino[])
{
    if (arquivo_fonte == NULL)
    {
        printf("[[ERRO]] Falha em leitura de arquivo: arquivo eh invalido. [enviar_fragmentos::obter_fragmento_disco]\n\n");

        return false;
    }

    // Desloca do início do arquivo até a posição onde está o fragmento a ser lido.
    if (fseek(arquivo_fonte, (long int) (id_fragmento * tam_fragmento), SEEK_SET) != 0)
    {
        printf("[[ERRO]] Falha no deslocamento do arquivo: fseek falhou. [enviar_fragmentos::obter_fragmento_disco]\n\n");

        return false;
    }

    if (fread(destino, sizeof(byte), n_bytes_leitura, arquivo_fonte) != n_bytes_leitura)
    {
        printf("[[ERRO]] Falha na leitura do arquivo: fread falhou. [enviar_fragmentos::obter_fragmento_disco]\n\n");

        return false;
    }

    return true;
}
