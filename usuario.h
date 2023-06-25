#ifndef USUARIO_H
#define USUARIO_H

#include "info_usuario.h"
#include "info_compartilhada.h"
#include "manipulador_arquivos.h"

// Tipo `void *` para condizer com a assinatura de `pthread_create`.
void *iniciar_usuario(info_compartilhada_t *compartilhado);

bool inicializar_usuario(info_usuario_t *informacoes_usuario, info_compartilhada_t *compartilhado);

// Avisa a todos os outros usuários que o usuário invocador desta função está conectado.
void conectar_usuario(const info_usuario_t *informacoes_usuario, info_compartilhada_t *compartilhado);

#endif // USUARIO_H