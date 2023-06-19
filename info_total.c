#include "info_total.h"

void inicializar_info_total(info_total_t *info_total, info_usuario_t *info_usuario, info_compartilhada_t *info_compartilhada)
{
    info_total->info_usuario = info_usuario;
    info_total->info_compartilhada = info_compartilhada;
}