#include "par_usuario_arquivo.h"

bool comparar_par_usuario_arquivo(const par_usuario_arquivo_t *a, const par_usuario_arquivo_t *b)
{
    if (a->id_usuario == b->id_usuario && a->id_arquivo == b->id_arquivo) return true;

    return false;
}