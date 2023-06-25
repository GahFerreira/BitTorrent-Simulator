#ifndef GERENCIAR_BUFFERS_H
#define GERENCIAR_BUFFERS_H

#include <stdio.h>
#include <stdbool.h>

#include "info_total.h"

void *gerenciar_buffers(info_total_t *info_total);

bool gravar_buffer_disco(FILE *arquivo_destino, buffer_t *buffer);

/*
    Função encarregada das funcionalidades requeridas por completar um arquivo.

    1. Mudar o estado do arquivo para COMPLETO.
    2. Avisar aos demais usuários que finalizou o arquivo.
*/
void completar_arquivo(info_compartilhada_t *info_compartilhada, info_arquivos_t *info_arquivos, const unsigned id_usuario, const unsigned id_arquivo, buffer_t *buffer);

#endif // GERENCIAR_BUFFERS_H