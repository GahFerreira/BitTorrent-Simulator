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
            if (buffers_usuario[i_arquivo] != NULL)
            {
                trancar_buffer(buffers_usuario[i_arquivo]);

                if (buffers_usuario[i_arquivo]->n_fragmentos_ausentes == 0)
                {
                    /*
                        TODO:
                          1. Gravar buffer no disco.
                          2. Checar se o arquivo está completo.
                          2.1 Se sim, marcar arquivo como completo e avisar demais usuários.
                          3. Reinicializar buffer e lista de fragmentos em necessidade
                    */
                }

                destrancar_buffer(buffers_usuario[i_arquivo]);
            }
        }

        meu_sleep(1000);
    }

    pthread_exit(NULL);

    return NULL;
}