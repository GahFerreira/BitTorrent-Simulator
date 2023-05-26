#include <stdio.h>
#include <pthread.h>
#include <dirent.h>

#include "usuario.h"
#include "info_compartilhada.h"

void *iniciar_usuario(info_compartilhada_t *compartilhado)
{
    unsigned meu_id = (unsigned) pthread_self();
    DIR *meu_diretorio;
    struct dirent *meu_arquivo;
    char nome_diretorio[compartilhado->max_caracteres_dir_usuario];
    sprintf(nome_diretorio, "./U%u", meu_id);

    // Abertura de diretório, checando erro.
    if ((meu_diretorio = opendir(nome_diretorio)) == NULL)
    {
        printf("ERRO: Falha em abrir diretorio do usuario %d. Usuario finalizado.\n", meu_id);
        pthread_exit(NULL);
    }

    //printf("Diretorio aberto\n");

    while ((meu_arquivo = readdir(meu_diretorio)) != NULL)
    {
        // Arquivos de retorno.
        if (strcmp(meu_arquivo->d_name, ".") || strcmp(meu_arquivo->d_name, "..")) continue;

        printf("Usuario %d: %s\n", meu_id, meu_arquivo->d_name);
    }

    closedir(meu_diretorio);

    pthread_exit(NULL);

    return NULL;
}