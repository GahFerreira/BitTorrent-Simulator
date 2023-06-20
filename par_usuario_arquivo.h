#ifndef PAR_USUARIO_ARQUIVO_H
#define PAR_USUARIO_ARQUIVO_H

#include <stdbool.h>

typedef struct par_usuario_arquivo par_usuario_arquivo_t;

struct par_usuario_arquivo
{
    unsigned usuario, id_arquivo;
};

bool comparar_par_usuario_arquivo(const par_usuario_arquivo_t *a, const par_usuario_arquivo_t *b);

#endif // PAR_USUARIO_ARQUIVO_H