#ifndef ENVIAR_FRAGMENTOS_H
#define ENVIAR_FRAGMENTOS_H

#include "info_total.h"

void *enviar_fragmentos(info_total_t *info_total);
bool obter_fragmento_disco(FILE *arquivo_fonte, const unsigned id_fragmento, const unsigned tam_fragmento, const unsigned n_bytes_leitura, byte destino[]);

#endif // ENVIAR_FRAGMENTOS_H