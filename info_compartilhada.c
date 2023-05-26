#include "info_compartilhada.h"

void construir_info_compartilhada(info_compartilhada_t *informacoes_compartilhadas, unsigned n_usuarios, unsigned n_arquivos)
{
    informacoes_compartilhadas->n_usuarios = n_usuarios;
    informacoes_compartilhadas->n_arquivos = n_arquivos;

    // Calcula o chão de `log_10(n_usuarios) +1`.
    unsigned max_caracteres_dir_usuario = 0;
    while (n_usuarios > 0) 
    {
        max_caracteres_dir_usuario++;
        n_usuarios /= 10;
    }
    // `+3` pois inclui os outros caracteres do diretório: "./U{numero_usuario}"
    informacoes_compartilhadas->max_caracteres_dir_usuario = max_caracteres_dir_usuario+3;
}