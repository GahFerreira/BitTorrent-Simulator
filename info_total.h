#ifndef INFO_TOTAL_H
#define INFO_TOTAL_H

#include "info_usuario.h"
#include "info_compartilhada.h"

typedef struct info_total info_total_t;

// `struct` criada apenas para passar como parâmetro para threads internas do usuário.
struct info_total
{
    info_usuario_t *info_usuario;
    info_compartilhada_t *info_compartilhada;
};
 
void inicializar_info_total(info_total_t *info_total, info_usuario_t *info_usuario, info_compartilhada_t *info_compartilhada);

#endif // INFO_TOTAL_H