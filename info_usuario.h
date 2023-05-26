#ifndef INFO_USUARIO_H
#define INFO_USUARIO_H

#include <stdlib.h>

typedef unsigned char bool;

struct info_usuario
{
    bool *lista_arquivos;
};

typedef struct info_usuario info_usuario_t;

void inicializar_info_usuario(info_usuario_t* iu, size_t n_arquivos)
{
    iu->lista_arquivos = (bool *) calloc(n_arquivos, sizeof(bool));
}

#endif // INFO_USUARIO_H
