#ifndef USUARIO_H
#define USUARIO_H

#include "info_compartilhada.h"

// Tipo `void *` para condizer com a assinatura de `pthread_create`.
void *iniciar_usuario(const info_compartilhada_t *compartilhado);

// Função que converte `fileX.Y` para `X-1`.
unsigned nome_arquivo_para_id(const char nome_arquivo[]);

#endif // USUARIO_H