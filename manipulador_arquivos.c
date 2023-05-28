#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

#include "manipulador_arquivos.h"

void construir_manipulador_arquivos(manipulador_arquivos_t *manipulador_arquivos, const char *nome_diretorio, const unsigned max_caracteres_nome_diretorio)
{
    manipulador_arquivos->nome_diretorio = (char *) calloc(max_caracteres_nome_diretorio, sizeof(char)); 
    strcpy(manipulador_arquivos->nome_diretorio, nome_diretorio);

    manipulador_arquivos->diretorio = NULL;
    manipulador_arquivos->propriedades_arquivo = NULL;
}

bool abrir_diretorio(manipulador_arquivos_t *manipulador_arquivos)
{
    // Abertura de diretório, checando erro.
    if ((manipulador_arquivos->diretorio = opendir(manipulador_arquivos->nome_diretorio)) == NULL)
    {
        printf("\nERRO: Falha em abrir diretorio %s. [manipulador_arquivos::abrir_diretorio]\n\n", manipulador_arquivos->nome_diretorio);

        return false;
    }

    return true;
}

void fechar_diretorio(manipulador_arquivos_t *manipulador_arquivos)
{
    closedir(manipulador_arquivos->diretorio);
    manipulador_arquivos->diretorio = NULL;
}