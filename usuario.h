#ifndef USUARIO_H
#define USUARIO_H

#include "info_compartilhada.h"

// Tipo `void *` para condizer com a assinatura de `pthread_create`.
void *iniciar_usuario(info_compartilhada_t *compartilhado);

#endif // USUARIO_H