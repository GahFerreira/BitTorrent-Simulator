#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "usuario.h"
#include "info_usuario.h"
#include "info_compartilhada.h"

void inicializar_parametros_invocacao(int argc, char *argv[], unsigned *n_usuarios, unsigned *n_max_arquivos, unsigned *tam_fragmento, unsigned *tam_buffer)
{
    switch(argc)
    {
        // Invocação de programa sem parâmetros - usar apenas para teste.
        case 1:
        {
            *n_usuarios = 3;
            *n_max_arquivos = 6;
            *tam_fragmento = 4;
            *tam_buffer = 8;

            printf("Usando valores de TESTE:\n");

            break;
        }

        // Usando todos os parâmetros de invocação.
        case 5:
        {
            printf("Usando valores dos parametros:\n");

            sscanf(argv[2], "%u", n_usuarios);
            sscanf(argv[3], "%u", n_max_arquivos);
            sscanf(argv[4], "%u", tam_fragmento);
            sscanf(argv[5], "%u", tam_buffer);

            break;
        }

        default:
        {
            printf("Numero de parametros incorreto.\n\
                    Programa finalizado.\n");

            exit(0);
        }
    }

    printf("Numero de Usuarios: %u\n", *n_usuarios);
    printf("Numero Maximo de Arquivos: %u\n", *n_max_arquivos);
    printf("Tamanho por Fragmento: %u bytes\n", *tam_fragmento);
    printf("Tamanho Buffer: %u fragmentos\n\n", *tam_buffer);
}

int main(int argc, char *argv[])
{
    printf("Inicio do programa\n\n");

    // Parâmetros de invocação.
    unsigned n_usuarios, n_max_arquivos, tam_fragmento, tam_buffer;
    inicializar_parametros_invocacao(argc, argv, &n_usuarios, &n_max_arquivos, &tam_fragmento, &tam_buffer);

    // Informações compartilhadas entre as threads.
    info_compartilhada_t compartilhado;
    construir_info_compartilhada(&compartilhado, n_usuarios, n_max_arquivos);

    // Instanciação das threads a serem usadas.
    pthread_t usuario[n_usuarios];

    // Inicia as threads.
    for (unsigned i_usuario = 0; i_usuario < n_usuarios; i_usuario++)
    {
        printf("Criacao do usuario %d\n", i_usuario+1);
        pthread_create(&usuario[i_usuario], NULL, (void * (*)(void *)) iniciar_usuario, (void *) &compartilhado);
    }
    printf("\n");

    // Espera todas as threads concluírem (necessário pois `compartilhado` é local da main).
    for (unsigned i_usuario = 0; i_usuario < n_usuarios; i_usuario++)
    {
        pthread_join(usuario[i_usuario], NULL);
        printf("Usuario %d terminou\n", i_usuario+1);
    }
    printf("\n");

    printf("Fim do programa\n");

    return 0;
}