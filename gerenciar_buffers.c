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
                        printf("[[ERRO]] Falha ao criar arquivo %s do usuario %u.\n\n", info_total->info_usuario->info_arquivos.nome_arquivos[i_arquivo], info_total->info_usuario->id_usuario+1);
                    }
                    
                    buffer_arquivo->arquivo_criado = true;

                    // Se o arquivo for finalizado, ou seja, o arquivo tem 0 bytes.
                    if (atualizar_buffer(buffer_arquivo) == true)
                    {
                        completar_arquivo(info_total->info_compartilhada, &info_total->info_usuario->info_arquivos, id_usuario, i_arquivo, buffer_arquivo);
                    }
                }

                else if (buffer_arquivo->n_fragmentos_ausentes == 0)
                {
                    gravar_buffer_disco(info_total->info_usuario->manipulador_arquivos.ponteiro_arquivos[i_arquivo], buffer_arquivo);

                    // Se o arquivo for finalizado, ou seja, o arquivo tem 0 bytes.
                    if (atualizar_buffer(buffer_arquivo) == true)
                    {
                        completar_arquivo(info_total->info_compartilhada, &info_total->info_usuario->info_arquivos, id_usuario, i_arquivo, buffer_arquivo);
                    }
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

    const unsigned qtd_bytes_para_gravar = obter_quantidade_bytes_para_gravar(buffer);

    if (fwrite(buffer->dados_fragmentos, sizeof(byte), qtd_bytes_para_gravar, arquivo_destino) != qtd_bytes_para_gravar)
    {
        printf("[[ERRO]] Falha na escrita do arquivo: fwrite falhou. [gerenciar_buffers::gravar_buffer_disco]\n\n");

        return false;
    }

    return true;
}

void completar_arquivo(info_compartilhada_t *info_compartilhada, info_arquivos_t *info_arquivos, const unsigned id_usuario, const unsigned id_arquivo, buffer_t *buffer)
{
    bool usuario_completo = false;

    trancar_info_arquivos(info_arquivos);

    mudar_arquivo_para_completo(info_arquivos, id_usuario, id_arquivo);

    if (info_arquivos->n_completos == info_arquivos->n_arquivos) usuario_completo = true;

    destrancar_info_arquivos(info_arquivos);

    if (usuario_completo == true)
    {
        novo_usuario_finalizado(info_compartilhada);
    }

    for (unsigned i_usuario = 0; i_usuario < info_compartilhada->n_usuarios; ++i_usuario)
    {
        enviar_mensagem_arquivo_completo(info_compartilhada, id_usuario, id_arquivo, i_usuario);
    }

    // TODO: Finalizar buffer. É mais difícil do que parece, por causa do mutex de buffer. Inclusive, ele está trancado neste momento.
}