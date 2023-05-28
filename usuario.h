#ifndef USUARIO_H
#define USUARIO_H

#include "info_usuario.h"
#include "info_compartilhada.h"
#include "manipulador_arquivos.h"

// Tipo `void *` para condizer com a assinatura de `pthread_create`.
void *iniciar_usuario(info_compartilhada_t *compartilhado);

bool inicializar_usuario(info_usuario_t *informacoes_usuario, const info_compartilhada_t *compartilhado, manipulador_arquivos_t *manipulador_arquivos);

void finalizar_usuario(info_usuario_t *minhas_informacoes, manipulador_arquivos_t *manipulador_arquivos);

// Funções de Utilidade

// Assume que destino tenha a capacidade necessária para guardar o nome calculado.
void id_usuario_para_nome_diretorio(char *destino, const unsigned id_usuario);

// Função que converte "fileX.Y" para o inteiro `X-1`.
// Presume-se que o nome do arquivo é SEMPRE "fileX.Y", tal que 'X' é um inteiro positivo.
unsigned nome_arquivo_para_id(const char nome_arquivo[]);

#endif // USUARIO_H