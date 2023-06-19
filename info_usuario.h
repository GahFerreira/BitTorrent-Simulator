/**
 * Estrutura de dados privada de cada usuário, com propósito de unificar as
 * informações daquele usuário.
*/

#ifndef INFO_USUARIO_H
#define INFO_USUARIO_H

#include <stdlib.h>
#include <stdbool.h>

#include "dado_concorrente.h"
#include "manipulador_arquivos.h"

typedef unsigned char estado_progresso_t;
typedef struct info_usuario info_usuario_t;
typedef struct info_arquivos info_arquivos_t;

enum estados_progresso {VAZIO = 0, EM_PROGRESSO = 1, COMPLETO = 2};

struct info_arquivos
{
    unsigned n_arquivos, n_vazios, n_em_progresso, n_completos;
    estado_progresso_t *estado_arquivos;
};

struct info_usuario
{
    unsigned id_usuario;

    info_arquivos_t info_arquivos;
    dado_concorrente_t controlador_info_arquivos;
};

void construir_info_usuario(info_usuario_t* informacoes_usuario, const unsigned id, const unsigned n_arquivos);

void construir_info_arquivos(info_arquivos_t *info_arquivos, const unsigned n_arquivos);

bool inicializar_info_arquivos(info_arquivos_t *info_arquivos, unsigned (funcao_conversora) (const char[]), manipulador_arquivos_t *manipulador_arquivos);

// Métodos de interação com info_arquivos.

/*
    As funções de abrir e fechar a lista são para controle de acesso concorrente.

    Em outras estruturas, é possível fazer esse controle internamente às funções,
    porém em `info_arquivos_t` é preciso fazer diversos acessos distintos para
    concluir uma operação. 
*/
void abrir_info_arquivos(dado_concorrente_t *controlador_info_arquivos); 
void fechar_info_arquivos(dado_concorrente_t *controlador_info_arquivos);

void obter_arquivos_em_progresso(const dado_concorrente_t *controlador_info_arquivos, unsigned arquivos_em_progresso[]);

#endif // INFO_USUARIO_H