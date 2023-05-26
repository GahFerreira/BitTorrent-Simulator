#ifndef INFO_COMPARTILHADA_H
#define INFO_COMPARTILHADA_H

#include "lista_encadeada.h"

typedef struct info_compartilhada info_compartilhada_t;

struct info_compartilhada
{
    unsigned n_usuarios;
    unsigned n_arquivos;
    
    // Usado para instanciar nomes de diretórios de usuários corretamente.
    unsigned max_caracteres_dir_usuario;
};

void construir_info_compartilhada(info_compartilhada_t *informacoes_compartilhadas, unsigned n_usuarios, unsigned n_arquivos);

#endif // INFO_COMPARTILHADA_H
