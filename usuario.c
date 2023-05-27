#include <stdio.h>
#include <pthread.h>
#include <dirent.h>

#include "usuario.h"
#include "info_usuario.h"
#include "info_compartilhada.h"

void *iniciar_usuario(const info_compartilhada_t *compartilhado)
{
    info_usuario_t minhas_informacoes;
    construir_info_usuario(&minhas_informacoes, (unsigned) pthread_self()-1, compartilhado->n_arquivos);

    DIR *meu_diretorio;
    struct dirent *meu_arquivo;
    char nome_diretorio[compartilhado->max_caracteres_dir_usuario];
    sprintf(nome_diretorio, "./U%u", minhas_informacoes.meu_id+1);
    
    // Abertura de diretório, checando erro.
    if ((meu_diretorio = opendir(nome_diretorio)) == NULL)
    {
        printf("ERRO: Falha em abrir diretorio do usuario %d. Usuario finalizado.\n", minhas_informacoes.meu_id+1);
        pthread_exit(NULL);
    }

    printf("Usuario %d: diretorio aberto\n", minhas_informacoes.meu_id+1);

    while ((meu_arquivo = readdir(meu_diretorio)) != NULL)
    {
        // Arquivos de retorno de diretório.
        if (strcmp(meu_arquivo->d_name, ".") == 0 || strcmp(meu_arquivo->d_name, "..") == 0) continue;

        printf("Usuario %d: %s, cujo id eh %d\n", minhas_informacoes.meu_id+1, meu_arquivo->d_name, nome_arquivo_para_id(meu_arquivo->d_name));
    }

    printf("Usuario %d: diretorio fechado\n", minhas_informacoes.meu_id+1);

    closedir(meu_diretorio);

    pthread_exit(NULL);

    return NULL; // Sem retorno.
}

// Presume-se que o nome do arquivo é SEMPRE "fileX.Y", tal que 'X' é um inteiro positivo.
unsigned nome_arquivo_para_id(const char nome_arquivo[])
{
    unsigned pos_ponto;

    // A primeira possível posição para o ponto é o 5o caractere do vetor.
    for (pos_ponto = 5; nome_arquivo[pos_ponto] != '.'; pos_ponto++);

    // Converte 'X' de char[] para unsigned.
    unsigned retorno = 0;
    for (unsigned at = 4; at < pos_ponto; at++)
    {
        retorno = retorno * 10 + ((unsigned) nome_arquivo[at] - '0');
    }

    // Retorna -1 porque o id do arquivo é 0-based, exceto nas operações com o diretório.
    return retorno-1;
}