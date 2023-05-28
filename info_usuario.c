#include <stdio.h>
#include <stdbool.h>

#include "info_usuario.h"

void construir_info_usuario(info_usuario_t* informacoes_usuario, const unsigned id, const unsigned n_arquivos)
{
    informacoes_usuario->id_usuario = id;
    informacoes_usuario->lista_arquivos = (bool *) calloc(n_arquivos, sizeof(bool));
}

bool inicializar_lista_arquivos(bool *lista_arquivos, unsigned (funcao_conversora) (const char[]), manipulador_arquivos_t *manipulador_arquivos)
{
    if (!abrir_diretorio(manipulador_arquivos))
    {
        printf("\nERRO: Falha em inicializar lista de arquivos do diretorio %s. [info_usuario::inicializar_lista_arquivos]\n\n", manipulador_arquivos->nome_diretorio);

        return false;
    }

    while ((manipulador_arquivos->propriedades_arquivo = readdir(manipulador_arquivos->diretorio)) != NULL)
    {
        // Arquivos "." e ".." são ignorados.
        if (strcmp(manipulador_arquivos->propriedades_arquivo->d_name, ".") == 0 ||
            strcmp(manipulador_arquivos->propriedades_arquivo->d_name, "..") == 0) continue;

        unsigned id_arquivo = funcao_conversora(manipulador_arquivos->propriedades_arquivo->d_name);

        lista_arquivos[id_arquivo] = true;
    }

    fechar_diretorio(manipulador_arquivos);

    return true;
}