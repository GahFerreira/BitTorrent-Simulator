#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>

#include "manipulador_arquivos.h"

void inicializar_manipulador_arquivos(manipulador_arquivos_t *manipulador_arquivos, const char *nome_diretorio, const unsigned max_caracteres_nome_diretorio, const unsigned n_arquivos)
{
    manipulador_arquivos->nome_diretorio = (char *) calloc(max_caracteres_nome_diretorio, sizeof(char));

    // Limpa `manipulador_arquivos->nome_diretorio`.
    for (unsigned i_char = 0; i_char < max_caracteres_nome_diretorio; ++i_char)
    {
        manipulador_arquivos->nome_diretorio[i_char] = '\0';
    }

    strncpy(manipulador_arquivos->nome_diretorio, nome_diretorio, max_caracteres_nome_diretorio+1);

    manipulador_arquivos->n_arquivos_diretorio = obter_numero_arquivos_diretorio(manipulador_arquivos);

    manipulador_arquivos->diretorio = NULL;
    manipulador_arquivos->propriedades_arquivo = NULL;

    manipulador_arquivos->ponteiro_arquivos = (FILE **) malloc(n_arquivos * sizeof(FILE *));
    for (unsigned i_arquivo = 0; i_arquivo < n_arquivos; ++i_arquivo)
    {
        manipulador_arquivos->ponteiro_arquivos[i_arquivo] = NULL;
    }
}

bool abrir_diretorio(manipulador_arquivos_t *manipulador_arquivos)
{
    // Abertura de diretório, checando erro.
    if ((manipulador_arquivos->diretorio = opendir(manipulador_arquivos->nome_diretorio)) == NULL)
    {
        printf("[[ERRO]] Falha em abrir diretorio %s. [manipulador_arquivos::abrir_diretorio]\n\n", manipulador_arquivos->nome_diretorio);

        return false;
    }

    return true;
}

void fechar_diretorio(manipulador_arquivos_t *manipulador_arquivos)
{
    closedir(manipulador_arquivos->diretorio);
    manipulador_arquivos->diretorio = NULL;
}

unsigned obter_numero_arquivos_diretorio(manipulador_arquivos_t *manipulador_arquivos)
{
    if (!abrir_diretorio(manipulador_arquivos))
    {
        printf("[[ERRO]] Falha em abrir diretorio %s. [manipulador_arquivos::obter_numero_arquivos_diretorio]\n\n", manipulador_arquivos->nome_diretorio);

        return 0;
    }

    unsigned i_arquivo = 0;

    while ((manipulador_arquivos->propriedades_arquivo = readdir(manipulador_arquivos->diretorio)) != NULL)
    {
        // Arquivos "." e ".." são ignorados.
        if (strcmp(manipulador_arquivos->propriedades_arquivo->d_name, ".") == 0 ||
            strcmp(manipulador_arquivos->propriedades_arquivo->d_name, "..") == 0) continue;

        ++i_arquivo;
    }

    fechar_diretorio(manipulador_arquivos);

    return i_arquivo;
}

unsigned obter_nomes_arquivos_diretorio(manipulador_arquivos_t *manipulador_arquivos, char **destino, const unsigned tam_max_nome_esperado)
{
    if (!abrir_diretorio(manipulador_arquivos))
    {
        printf("[[ERRO]] Falha em inicializar lista de arquivos do diretorio %s. [manipulador_arquivos::obter_nomes_arquivos_diretorio]\n\n", manipulador_arquivos->nome_diretorio);

        return 0;
    }

    unsigned i_arquivo = 0, tam_nome, tam_maior_nome = 0;

    while ((manipulador_arquivos->propriedades_arquivo = readdir(manipulador_arquivos->diretorio)) != NULL)
    {
        // Arquivos "." e ".." são ignorados.
        if (strcmp(manipulador_arquivos->propriedades_arquivo->d_name, ".") == 0 ||
            strcmp(manipulador_arquivos->propriedades_arquivo->d_name, "..") == 0) continue;

        tam_nome = (unsigned) strlen(manipulador_arquivos->propriedades_arquivo->d_name);
        
        if (tam_nome > tam_maior_nome) tam_maior_nome = tam_nome;

        strncpy(destino[i_arquivo], manipulador_arquivos->propriedades_arquivo->d_name, tam_max_nome_esperado);

        ++i_arquivo;
    }

    fechar_diretorio(manipulador_arquivos);

    return tam_maior_nome;
}

bool criar_arquivo_diretorio(manipulador_arquivos_t *manipulador_arquivos, const char nome_arquivo[], const unsigned id_arquivo, const unsigned tam_arquivo)
{
    if (manipulador_arquivos->ponteiro_arquivos[id_arquivo] != NULL)
    {
        printf("[[ERRO]] Falha em criar arquivo %u: arquivo ja existe.\n\n", id_arquivo+1);

        return false;
    }

    manipulador_arquivos->ponteiro_arquivos[id_arquivo] = fopen(nome_arquivo, "wb");

    // //fdopen associa um FILE* a um int file descriptor
    // //acho que rb+ é válido
    // FILE* arquivo = fdopen(fd,"rb+");
    // ftruncate(fileno(arquivo), tam_arquivo);



}