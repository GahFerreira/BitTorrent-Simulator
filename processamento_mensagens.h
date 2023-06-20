/*
    Funções relacionadas à Thread 1 do usuário.

    Responsável por mandar mensagens a outros usuários
    e processar as mensagens recebidas de outros usuários.
*/

#ifndef PROCESSAMENTO_MENSAGENS_H
#define PROCESSAMENTO_MENSAGENS_H

#include "info_total.h"
#include "lista_mensagem.h"

void *processar_mensagens_recebidas(info_total_t *info_total);

void checar_novo_usuario_conectado(info_total_t *info_total, lista_mensagem_t *novos_usuarios_conectados, const unsigned id_usuario);
void checar_mensagem_arquivo_completo(info_total_t *info_total, lista_mensagem_t *arquivo_completo, const unsigned id_usuario);
void checar_solicitacoes_arquivo(info_total_t *info_total, lista_mensagem_t *solicitacoes_arquivo, const unsigned id_usuario);

#endif // PROCESSAMENTO_MENSAGENS_H