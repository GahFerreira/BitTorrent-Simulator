/*
    Funções relacionadas à Thread 2 do usuário.

    Responsável por enviar solicitações de arquivos a outros usuários,
    de forma aleatória.
*/

#ifndef SOLICITAR_ARQUIVOS_H
#define SOLICITAR_ARQUIVOS_H

#include "info_total.h"

void *solicitar_arquivos(info_total_t *info_total);
void solicitar_arquivo(info_compartilhada_t *info_compartilhada, info_arquivos_t *info_arquivos, const unsigned id_usuario, const unsigned id_arquivo);

#endif // SOLICITAR_ARQUIVOS_H