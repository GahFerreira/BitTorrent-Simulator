/**
 * Estrutura de dados privada de cada usuário, com propósito de unificar as
 * informações daquele usuário.
*/

#ifndef INFO_USUARIO_H
#define INFO_USUARIO_H

#include <stdlib.h>

typedef unsigned char bool;

struct info_usuario
{
    unsigned meu_id;

    bool *lista_arquivos;
};

typedef struct info_usuario info_usuario_t;

void construir_info_usuario(info_usuario_t* informacoes_usuario, const unsigned id, const unsigned n_arquivos);

#endif // INFO_USUARIO_H