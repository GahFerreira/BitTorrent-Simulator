#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <dirent.h>

#include "usuario.h"
#include "info_usuario.h"
#include "manipulador_arquivos.h"
#include "util.h"

void *iniciar_usuario(info_compartilhada_t *compartilhado)
{
    // printf("Usuario %d - Parte 1\n", (unsigned) pthread_self());
    /// PARTE 1: Inicializações de estruturas de dados.

    info_usuario_t minhas_informacoes;
    manipulador_arquivos_t manipulador_arquivos;

    if (!inicializar_usuario(&minhas_informacoes, compartilhado, &manipulador_arquivos))
    {
        printf("\nERRO: Falha em inicializar usuario %u. Finalizando usuario. [usuario::iniciar_usuario]\n\n", (unsigned) pthread_self());

        pthread_exit(NULL);
    }

    // printf("Usuario %u - Parte 2\n", (unsigned) pthread_self());
    /// PARTE 2: Avisa que se conectou aos outros usuários.
    conectar_usuario(&minhas_informacoes, compartilhado);

    // printf("Usuario %u - Passou pela conexao\n", (unsigned) pthread_self());

    // TESTE
    pthread_mutex_lock(&compartilhado->usuarios_conectados.mutex_mensagem);
    // printf("Usuario %u - Lista conectados:\n", minhas_informacoes.id_usuario+1);
    no_t *aux = compartilhado->usuarios_conectados.mensagens.primeiro;
    // printf("Usuario %u - %u usuarios conectados\n", minhas_informacoes.id_usuario+1, compartilhado->usuarios_conectados.mensagens.tamanho);
    while (true)
    {
        printf("\t{%u}\n", *((const unsigned *) aux->dado));
        if (aux->proximo == NULL) break;
        aux = aux->proximo;
    }
    printf("\n");
    pthread_mutex_unlock(&compartilhado->usuarios_conectados.mutex_mensagem);

    _sleep(3000);

    // Usuário remove a sua conexão do programa.
    remover_elemento_lista_mensagens(&compartilhado->usuarios_conectados, &minhas_informacoes.id_usuario, (bool (*) (const void *, const void *)) comparar_unsigned);

    /// PARTE X: Destruir estruturas de dados.
    //finalizar_usuario(&minhas_informacoes, &manipulador_arquivos);
    pthread_exit(NULL);

    return NULL; // Sem retorno.
}

bool inicializar_usuario(info_usuario_t *informacoes_usuario, const info_compartilhada_t *compartilhado, manipulador_arquivos_t *manipulador_arquivos)
{
    construir_info_usuario(informacoes_usuario, (unsigned) pthread_self()-1, compartilhado->n_arquivos);

    // Novo escopo temporário para evitar múltiplas alocações dinâmicas.
    {
        char nome_diretorio[compartilhado->max_caracteres_dir_usuario];
        id_usuario_para_nome_diretorio(nome_diretorio, informacoes_usuario->id_usuario);

        construir_manipulador_arquivos(manipulador_arquivos, nome_diretorio, compartilhado->max_caracteres_dir_usuario);
    }

    if (!inicializar_lista_arquivos(informacoes_usuario->lista_arquivos, nome_arquivo_para_id, manipulador_arquivos))
    {
        printf("\nERRO: Falha em inicializar lista de arquivos do usuario %u. [usuario::inicializar_usuario]\n\n", informacoes_usuario->id_usuario+1);

        return false;
    }

    return true;
}

// void finalizar_usuario(info_usuario_t *minhas_informacoes, manipulador_arquivos_t *manipulador_arquivos)
// {

// }

// A implementação usa diretamente as funções de lista encadeada, pois precisa travar
// o mutex para realizar muitas operações.
void conectar_usuario(const info_usuario_t *informacoes_usuario, info_compartilhada_t *compartilhado)
{
    // printf("Usuario %u - Inicio da conexao\n", informacoes_usuario->id_usuario+1);

    // Inicialmente, obtém o lock da lista de usuários conectados para obter os seus dados
    // e se incluir nela posteriormente.

    // printf("Usuario %u - 1\n", informacoes_usuario->id_usuario+1);
    // LOCK
    pthread_mutex_lock(&compartilhado->usuarios_conectados.mutex_mensagem);

    // printf("Usuario %u - Obteve o lock\n", informacoes_usuario->id_usuario+1);
    
    // printf("Usuario %u - 2\n", informacoes_usuario->id_usuario+1);
    unsigned n_conectados = compartilhado->usuarios_conectados.mensagens.tamanho;
    unsigned *usuarios_conectados[n_conectados];

    // printf("Usuario %u - 3\n", informacoes_usuario->id_usuario+1);
    obter_dados_lista_encadeada(&compartilhado->usuarios_conectados.mensagens, (const void **) &usuarios_conectados);

    // printf("Usuario %u - 4\n", informacoes_usuario->id_usuario+1);
    adicionar_elemento_lista_encadeada(&compartilhado->usuarios_conectados.mensagens, &informacoes_usuario->id_usuario);

    // printf("Usuario %u - Concedeu o lock\n", informacoes_usuario->id_usuario+1);
    // printf("Usuario %u - 5\n", informacoes_usuario->id_usuario+1);
    pthread_mutex_unlock(&compartilhado->usuarios_conectados.mutex_mensagem);
    // UNLOCK
    // Posteriormente, fora do lock da lista compartilhada, trata o envio de mensagens
    // aos outros usuários, usando o lock de cada lista individual.
    for (unsigned i_conectado = 0; i_conectado < n_conectados; i_conectado++)
    {
        // printf("Usuario %u - Entra no for\n", informacoes_usuario->id_usuario+1);
        unsigned usuario_atual = *usuarios_conectados[i_conectado];
        //printf("Usuario %u - Tenta mandar mensagem ao usuario %u\n", informacoes_usuario->id_usuario+1, usuario_atual+1);
        
        adicionar_elemento_lista_mensagens(&compartilhado->novo_usuario_conectado[usuario_atual], &informacoes_usuario->id_usuario);

        //printf("Usuario %u - Termina de mandar mensagem ao usuario %u\n", informacoes_usuario->id_usuario+1, usuario_atual+1);
    }
    //printf("Usuario %u - Fim da Funcao conectar_usuario\n", informacoes_usuario->id_usuario+1);
}

void id_usuario_para_nome_diretorio(char *destino, const unsigned id_usuario)
{
    sprintf(destino, "./U%u", id_usuario+1);
}

unsigned nome_arquivo_para_id(const char nome_arquivo[])
{
    unsigned pos_ponto;

    // A primeira possível posição para o ponto é o 5o caractere do vetor.
    for (pos_ponto = 5; nome_arquivo[pos_ponto] != '.'; pos_ponto++);

    // Converte 'X' de char[] para unsigned.
    unsigned retorno = 0;
    for (unsigned at = 4; at < pos_ponto; at++)
    {
        retorno = retorno * 10 + ((unsigned) nome_arquivo[at] - '0');
    }

    // Retorna -1 porque o id do arquivo é 0-based, exceto nas operações com o diretório.
    return retorno-1;
}