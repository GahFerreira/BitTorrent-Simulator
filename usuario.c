#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <dirent.h>

#include "usuario.h"
#include "info_usuario.h"
#include "manipulador_arquivos.h"
#include "util.h"
#include "processamento_mensagens.h"
#include "solicitar_arquivos.h"
#include "gerenciar_buffers.h"
#include "enviar_fragmentos.h"

void *iniciar_usuario(info_compartilhada_t *info_compartilhada)
{
    /* PARTE 1: Inicializações de estruturas de dados. */
    info_usuario_t info_usuario;

    if (!inicializar_usuario(&info_usuario, info_compartilhada))
    {
        // O id_usuario é garantido de ser inicializado.
        printf("[[ERRO]] Falha em inicializar usuario %u. Finalizando usuario. [usuario::iniciar_usuario]\n\n", info_usuario.id_usuario);

        // TODO: finalizar_usuario()

        pthread_exit(NULL);
    }

    /* PARTE 2: Realiza os procedimentos de conexão. */
    conectar_usuario(&info_usuario, info_compartilhada);

    /* PARTE 3: Inicia a rotina principal. */

    info_total_t info_total;
    info_total.info_compartilhada = info_compartilhada;
    info_total.info_usuario = &info_usuario;

    pthread_t th_processar_mensagens, th_solicitar_arquivos, th_gerenciar_buffers, th_enviar_fragmentos;

    pthread_create(&th_processar_mensagens, NULL, (void * (*) (void *)) processar_mensagens_recebidas, (void *) &info_total);

    pthread_create(&th_solicitar_arquivos, NULL, (void * (*) (void *)) solicitar_arquivos, (void *) &info_total);

	pthread_create(&th_gerenciar_buffers, NULL, (void * (*) (void *)) gerenciar_buffers, (void *) &info_total);

	pthread_create(&th_enviar_fragmentos, NULL, (void * (*) (void *)) enviar_fragmentos, (void *) &info_total);

    pthread_join(th_processar_mensagens, NULL);
    pthread_join(th_solicitar_arquivos, NULL);
    pthread_join(th_gerenciar_buffers, NULL);
    pthread_join(th_enviar_fragmentos, NULL);

    meu_sleep(3000);

    // Usuário remove a sua conexão do programa.
    printf("Usuario %u comeca a desconectar.\n\n", info_usuario.id_usuario);

    extrair_elemento_lista_mensagem(&info_compartilhada->usuarios_conectados, &info_usuario.id_usuario, (bool (*) (const void *, const void *)) comparar_unsigned);



    /// PARTE X: Destruir estruturas de dados.
    //finalizar_usuario(&info_usuario, &manipulador_arquivos);
    pthread_exit(NULL);

    return NULL; // Sem retorno.
}

bool inicializar_usuario(info_usuario_t *informacoes_usuario, info_compartilhada_t *info_compartilhada)
{
	#if DEBUG >= 4
	printf("[DEBUG-3] Novo usuario a ser inicializado. Id da sua thread: %u\n\n", (unsigned) pthread_self()-1);
	#endif

    if ( inicializar_info_usuario(informacoes_usuario, obter_id_prox_usuario(info_compartilhada), info_compartilhada->n_arquivos, info_compartilhada->max_caracteres_dir_usuario) == false )
    {
        printf("[[ERRO]] Falha em inicializar info_usuario do usuario %u.[usuario::inicializar_usuario]\n\n", informacoes_usuario->id_usuario+1);

        return false;
    }

    return true;
}

// void finalizar_usuario(info_usuario_t *info_usuario, manipulador_arquivos_t *manipulador_arquivos)
// {

// }

/* 
  A implementação usa diretamente as funções de lista encadeada, pois precisa travar
  o mutex para realizar muitas operações.
*/
void conectar_usuario(const info_usuario_t *informacoes_usuario, info_compartilhada_t *info_compartilhada)
{
    #if DEBUG >= 4
    printf("[DEBUG-3] Inicio da conexao do usuario %u\n\n", informacoes_usuario->id_usuario+1);
    #endif

    /* 
      Inicialmente, obtém o lock da lista de usuários conectados 
      para descobrir quem está conectado e se incluir na lista. 
    */

    // LOCK
    pthread_mutex_lock(&info_compartilhada->usuarios_conectados.mutex_mensagem);

    unsigned n_conectados = info_compartilhada->usuarios_conectados.mensagens.tamanho;

    /*  
      Vetor de ponteiros para unsigned. 
      Após sua inicialização, cada posição apontará para o id de um usuário conectado.
    */
    unsigned *usuarios_conectados[n_conectados];

    obter_dados_lista_encadeada(&info_compartilhada->usuarios_conectados.mensagens, (const void **) &usuarios_conectados);

    // O usuário adiciona a si próprio na lista compartilhada de usuários conectados.
    adicionar_elemento_lista_encadeada(&info_compartilhada->usuarios_conectados.mensagens, &informacoes_usuario->id_usuario);

    #if DEBUG >= 4
    printf("[DEBUG-3] Usuario %u parcialmente conectado. Avisando aos outros usuarios sobre sua conexao.\n\n", informacoes_usuario->id_usuario+1);
    #endif

    pthread_mutex_unlock(&info_compartilhada->usuarios_conectados.mutex_mensagem);
    // UNLOCK

    /* 
      Posteriormente, fora do lock da lista compartilhada, trata o envio
      de mensagens aos outros usuários usando o lock de cada lista individual.
    */
    for (unsigned i_conectado = 0; i_conectado < n_conectados; ++i_conectado)
    {
        // Obtém o id de um dos usuários conectados.
        unsigned usuario_atual = *usuarios_conectados[i_conectado];
        
        /* 
          O usuário envia mensagem a esse usuário conectado, 
          dizendo que agora também está conectado. 
        */
        adicionar_elemento_lista_mensagem(&info_compartilhada->novos_usuarios_conectados[usuario_atual], &informacoes_usuario->id_usuario);
    }

    #if DEBUG >= 4
    printf("[DEBUG-3] Usuario %u totalmente conectado. Demais usuarios avisados sobre sua conexao.\n\n", informacoes_usuario->id_usuario+1);
    #endif
}