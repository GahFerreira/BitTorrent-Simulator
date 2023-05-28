#ifndef MANIPULADOR_ARQUIVOS_H
#define MANIPULADOR_ARQUIVOS_H

#include <stdbool.h>
#include <dirent.h>

typedef struct manipulador_arquivos manipulador_arquivos_t;

struct manipulador_arquivos
{
    char *nome_diretorio;

    // Manipulador de diretórios.
    DIR *diretorio;
    // Extrator do nome dos arquivos de um diretório.
    struct dirent *propriedades_arquivo;
};

void construir_manipulador_arquivos(manipulador_arquivos_t *manipulador_arquivos, const char *nome_diretorio, const unsigned max_caracteres_nome_diretorio);

bool abrir_diretorio(manipulador_arquivos_t *manipulador_arquivos);
void fechar_diretorio(manipulador_arquivos_t *manipulador_arquivos);

#endif // MANIPULADOR_ARQUIVOS_H