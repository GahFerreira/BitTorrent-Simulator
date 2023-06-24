#ifndef MANIPULADOR_ARQUIVOS_H
#define MANIPULADOR_ARQUIVOS_H

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

typedef struct manipulador_arquivos manipulador_arquivos_t;

struct manipulador_arquivos
{
    char *nome_diretorio;
    unsigned n_arquivos_diretorio;

    // Manipulador de diretórios.
    DIR *diretorio;
    // Extrator do nome dos arquivos de um diretório.
    struct dirent *propriedades_arquivo;

    FILE **ponteiro_arquivos;
};

void inicializar_manipulador_arquivos(manipulador_arquivos_t *manipulador_arquivos, const char *nome_diretorio, const unsigned max_caracteres_nome_diretorio, const unsigned n_arquivos);

bool abrir_diretorio(manipulador_arquivos_t *manipulador_arquivos);
void fechar_diretorio(manipulador_arquivos_t *manipulador_arquivos);

unsigned obter_numero_arquivos_diretorio(manipulador_arquivos_t *manipulador_arquivos);

/* 
  Obtém a lista de arquivos do diretório.
  Coloca o nome de cada arquivo em uma posição de `destino`, (exceto "." e "..").
  Presume que `destino` tenha pelo menos uma posição para cada nome de arquivo.

  Retorno: 
    - Em caso de falha de abertura do diretório, retorna 0.
    - Caso contrário, retorna o tamanho do maior nome do diretório.

  Mitigação de erro - Tamanho de nome de arquivo grande demais:
    Como o tamanho máximo de nome de arquivo é indefinido, a função recebe um `unsigned` (tam_nome_esperado) que define o tamanho alocado para cada posição de `destino.`
    Se o tamanho de algum arquivo do diretório for maior que o tamanho esperado,
    a função cessa sua execução.
*/
unsigned obter_nomes_arquivos_diretorio(manipulador_arquivos_t *manipulador_arquivos, char **destino, const unsigned tam_max_nome_esperado);

// Iteração com arquivos

bool criar_arquivo_diretorio(manipulador_arquivos_t *manipulador_arquivos, const char nome_arquivo[], const unsigned id_arquivo, const unsigned tam_arquivo);

#endif // MANIPULADOR_ARQUIVOS_H