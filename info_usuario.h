/**
 * Estrutura de dados privada de cada usuário, com propósito de unificar as
 * informações daquele usuário.
*/

#ifndef INFO_USUARIO_H
#define INFO_USUARIO_H

#include <stdlib.h>
#include <stdbool.h>

#include "manipulador_arquivos.h"

typedef struct info_usuario info_usuario_t;

struct info_usuario
{
    unsigned id_usuario;

    bool *lista_arquivos;
};

void construir_info_usuario(info_usuario_t* informacoes_usuario, const unsigned id, const unsigned n_arquivos);

bool inicializar_lista_arquivos(bool *lista_arquivos, unsigned (funcao_conversora) (const char[]), manipulador_arquivos_t *manipulador_arquivos);

#endif // INFO_USUARIO_H