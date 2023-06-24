#include "gerenciar_buffers.h"
#include "util.h" // TODO: Verificar uso de semáforos (ou auxiliar) para acordar função quando buffer encher.

void *gerenciar_buffers(info_total_t *info_total)
{
    const unsigned id_usuario = info_total->info_usuario->id_usuario;
    const unsigned n_arquivos = info_total->info_compartilhada->n_arquivos;

    buffer_t **buffers_usuario = info_total->info_compartilhada->buffers_usuarios[id_usuario];

    while (info_total->info_compartilhada->finalizar_execucao == false)
    {
        for (unsigned i_arquivo = 0; i_arquivo < n_arquivos; ++i_arquivo)
        {
            buffer_t *buffer_arquivo = buffers_usuario[i_arquivo];

            if (buffer_arquivo != NULL)
            {
                trancar_buffer(buffer_arquivo);

                /*
                    Quando um usuário solicita um arquivo, primeiramente ele requisita os seus dados.

                    Assim que os dados são colocados no buffer por outro usuário que possua aquele
                    arquivo, este usuário cria o arquivo com nome e tamanho correto.
                */
                if (buffer_arquivo->arquivo_criado == false && buffer_arquivo->dados_arquivo_obtidos == true)
                {
                    info_total->info_usuario->info_arquivos.tamanho_arquivos[i_arquivo] = buffer_arquivo->tam_arquivo;

                    strcpy(info_total->info_usuario->info_arquivos.nome_arquivos[i_arquivo], buffer_arquivo->nome_arquivo); 

                    // Cria o arquivo.
                    if ( criar_arquivo_diretorio(&info_total->info_usuario->manipulador_arquivos, info_total->info_usuario->info_arquivos.nome_arquivos[i_arquivo], i_arquivo, buffer_arquivo->tam_arquivo) == false )
                    {
                        printf("[[ERRO]] Falha ao criar arquivo %s do usuario %u.\n\n", info_total->info_usuario->info_arquivos.nome_arquivos[i_arquivo], info_total->info_usuario->id_usuario);
                    }

                    /*
                        TODO:
                          3. Reiniciar o buffer
                    */

                    buffer_arquivo->arquivo_criado = true;
                }

                if (buffer_arquivo->n_fragmentos_ausentes == 0)
                {
                    //info_total->info_usuario->manipulador_arquivos;
                    /*
                        TODO:
                          1. Gravar buffer no disco.
                          2. Checar se o arquivo está completo.
                          2.1 Se sim, marcar arquivo como completo e avisar demais usuários.
                          3. Reinicializar buffer e lista de fragmentos em necessidade
                    */
                    //gravar_buffer_disco();
                }

                destrancar_buffer(buffer_arquivo);
            }
        }

        meu_sleep(1000);
    }

    pthread_exit(NULL);

    return NULL;
}

bool gravar_buffer_disco(FILE *arquivo_destino, buffer_t *buffer)
{
    if (arquivo_destino == NULL)
    {
        printf("[[ERRO]] Falha em gravar buffer: arquivo invalido. [gerenciar_buffers::gravar_buffer_disco]\n\n");

        return false;
    }

    // Desloca do início do arquivo até a posição onde está o primeiro fragmento a ser escrito.
    if (fseek(arquivo_destino, (long int) (buffer->id_primeiro_fragmento_atual * buffer->tam_fragmento), SEEK_SET) != 0)
    {
        printf("[[ERRO]] Falha em se deslocar no arquivo. [gerenciar_buffers::gravar_buffer_disco]\n\n");

        return false;
    }

    

    // if (fwrite(buffer->dados_fragmentos, buffer->tam_fragmento, , arquivo_destino) != 1)
    // {
    //     printf("\narquivo::gravar_fragmento: Falha na escrita do arquivo.\n\n");

    //     return false;
    // }

    return true;
}