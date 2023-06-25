#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "util.h"

bool comparar_unsigned(const unsigned *a, const unsigned *b)
{
    if (*a == *b) return true;
    return false;
}

void meu_sleep(unsigned milisegundos)
{
    // Windows
    #ifdef _WIN32
    Sleep(milisegundos);

    // `nanosleep` está no posix.
    #else _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milisegundos / 1000;
    ts.tv_nsec = (milisegundos % 1000) * 1000000;
    nanosleep(&ts, NULL);
    #endif
}

void semear_numeros_aleatorios(void)
{
    srand((unsigned) time(NULL));
}

unsigned aleatorio(unsigned minimo, unsigned maximo)
{
    return minimo + ( (unsigned) rand() % (maximo-minimo+1) );
}

void id_usuario_para_nome_diretorio(char *destino, const unsigned id_usuario)
{
    sprintf(destino, "./U%u", id_usuario+1);
}

unsigned nome_arquivo_para_id(const char nome_arquivo[])
{
    unsigned pos_ponto;

    // A primeira possível posição para o ponto é o 5o caractere do vetor.
    for (pos_ponto = 5; nome_arquivo[pos_ponto] != '.'; ++pos_ponto);

    // Converte 'X' de char[] para unsigned.
    unsigned retorno = 0;
    for (unsigned at = 4; at < pos_ponto; ++at)
    {
        retorno = retorno * 10 + ((unsigned) nome_arquivo[at] - '0');
    }

    // Retorna -1 porque o id do arquivo é 0-based, exceto nas operações com o diretório.
    return retorno - 1;
}

void unir_nome_diretorio_arquivo(const char nome_diretorio[], const char nome_arquivo[], char caminho_arquivo[])
{
    #if DEBUG >= 7
    printf("[DEBUG-7] Unindo diretorio %s com arquivo %s.\n\n", nome_diretorio, nome_arquivo);
    #endif

    unsigned tam_nome_diretorio = (unsigned) strlen(nome_diretorio);

    /*
        Limpa `caminho_arquivo`. 

        TODO: Refatorar. Funciona sem `for`, mas é perigoso confiar em strings em c.
    */
    for (unsigned i_char = 0; i_char < tam_nome_diretorio + 256 + 5; ++i_char) caminho_arquivo[i_char] = '\0';

    strncpy(caminho_arquivo, nome_diretorio, tam_nome_diretorio+1);
    strncat(caminho_arquivo, "/", 1);
    strncat(caminho_arquivo, nome_arquivo, 256);

    #if DEBUG >= 7
    printf("[DEBUG-7] Resultado uniao do diretorio %s com arquivo %s: %s.\n\n", nome_diretorio, nome_arquivo, caminho_arquivo);
    #endif
}