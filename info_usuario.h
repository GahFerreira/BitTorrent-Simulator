/**
 * Estrutura de dados privada de cada usuário, com propósito de unificar as
 * informações daquele usuário.
*/

#ifndef INFO_USUARIO_H
#define INFO_USUARIO_H

#include <stdlib.h>
#include <stdbool.h>

#include "manipulador_arquivos.h"
#include "lista_mensagem.h"

typedef unsigned char estado_progresso_t;
typedef struct info_usuario info_usuario_t;
typedef struct info_arquivos info_arquivos_t;

enum estados_progresso {VAZIO = 0, EM_PROGRESSO = 1, COMPLETO = 2};

struct info_arquivos
{
    unsigned n_arquivos, n_vazios, n_em_progresso, n_completos;

    estado_progresso_t *estado_arquivos;
    unsigned *tamanho_arquivos;

    pthread_mutex_t mutex_info_arquivos;
};

struct info_usuario
{
    unsigned id_usuario;

    /*
        `info_arquivos` guarda as informações dos arquivos do usuário.

        Essas informações incluem o número de arquivos em cada estado
        e o estado de cada arquivo.

        É necessário trancar `info_arquivos` para fazer o acesso às suas variáveis.
    */
    info_arquivos_t info_arquivos;

    /*
        `manipulador_arquivos` guarda as informações do diretório do usuário e oferece funções para
        manipular os arquivos diretamente, como leituras e escritas.
    */
    manipulador_arquivos_t manipulador_arquivos;

    /*
        `lista_tarefas` guarda pares <id_usuario, id_arquivo> representando que o usuário atual está
        ajudando o usuário de id `id_usuario` a completar o arquivo de id `id_arquivo`.
    */
    lista_mensagem_t lista_tarefas;
};

bool inicializar_info_usuario(info_usuario_t *info_usuario, const unsigned id_usuario, const unsigned n_arquivos, const unsigned max_caracteres_dir_usuario);

bool inicializar_info_arquivos(info_arquivos_t *info_arquivos, manipulador_arquivos_t *manipulador_arquivos, const unsigned n_arquivos);

bool inicializar_dados_arquivos(info_arquivos_t *info_arquivos, unsigned (funcao_conversora) (const char[]), manipulador_arquivos_t *manipulador_arquivos);

// Métodos de interação com info_arquivos.

// Tranca o mutex de info_arquivos.
void trancar_info_arquivos(info_arquivos_t *info_arquivos);
// Destranca o mutex de info_arquivos.
void destrancar_info_arquivos(info_arquivos_t *info_arquivos);

// Função interna que só deve ser chamada com o mutex de info_arquivos bloqueado.
void obter_arquivos_generico(info_arquivos_t *info_arquivos, unsigned vetor_arquivos[], estado_progresso_t estado_escolhido);
// Preenche cada posição de `arquivos_faltantes` com o id de um arquivo que está `VAZIO`.
void obter_arquivos_ausentes(info_arquivos_t *info_arquivos, unsigned arquivos_faltantes[]);
// Preenche cada posição de `arquivos_em_progresso` com o id de um arquivo que está `EM_PROGRESSO`.
void obter_arquivos_em_progresso(info_arquivos_t *info_arquivos, unsigned arquivos_em_progresso[]);
// Preenche cada posição de `arquivos_completos` com o id de um arquivo que está `COMPLETO`.
void obter_arquivos_completos(info_arquivos_t *info_arquivos, unsigned arquivos_completos[]);

estado_progresso_t obter_estado_arquivo(info_arquivos_t *info_arquivos, const unsigned id_arquivo);

bool mudar_arquivo_para_em_progresso(info_arquivos_t *info_arquivos, const unsigned id_usuario, const unsigned id_arquivo);
// bool arquivo_para_completo();

// Métodos de iteração com `lista_tarefa`.

/*
    Adiciona a tarefa <id_usuario_tarefa, id_arquivo_tarefa> à lista de tarefas
    do usuário cujo id é `id_usuario`.

    Nota: para cada chamada de `adicionar_tarefa`, deve-se ter uma chamada de `completar_tarefa`.
*/
void adicionar_tarefa(lista_mensagem_t *lista_tarefa, const unsigned id_usuario, const unsigned id_usuario_tarefa, const unsigned id_arquivo_tarefa);
/*
    Completa a tarefa <id_usuario_tarefa, id_arquivo_tarefa> e a remove da lista de tarefas
    do usuário cujo id é `id_usuario`.

    Nota: para cada chamada de `adicionar_tarefa`, deve-se ter uma chamada de `completar_tarefa`.
*/
void completar_tarefa(lista_mensagem_t *lista_tarefa, const unsigned id_usuario, const unsigned id_usuario_tarefa, const unsigned id_arquivo_tarefa);

#endif // INFO_USUARIO_H