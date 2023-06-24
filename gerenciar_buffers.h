#ifndef GERENCIAR_BUFFERS_H
#define GERENCIAR_BUFFERS_H

#include <stdio.h>
#include <stdbool.h>

#include "info_total.h"

void *gerenciar_buffers(info_total_t *info_total);

bool gravar_buffer_disco(FILE *arquivo_destino, buffer_t *buffer);

#endif // GERENCIAR_BUFFERS_H