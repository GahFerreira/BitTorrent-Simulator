#include "info_usuario.h"

void construir_info_usuario(info_usuario_t* informacoes_usuario, const unsigned id, const unsigned n_arquivos)
{
    informacoes_usuario->meu_id = id;
    informacoes_usuario->lista_arquivos = (bool *) calloc(n_arquivos, sizeof(bool));
}