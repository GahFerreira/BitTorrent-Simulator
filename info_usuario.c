#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "info_usuario.h"

void construir_info_usuario(info_usuario_t *informacoes_usuario, const unsigned id, const unsigned n_arquivos)
{
    informacoes_usuario->id_usuario = id;
    informacoes_usuario->lista_arquivos = (estado_progresso_t *) calloc(n_arquivos, sizeof(estado_progresso_t));
}

bool inicializar_lista_arquivos(estado_progresso_t *lista_arquivos, unsigned (funcao_conversora) (const char[]), manipulador_arquivos_t *manipulador_arquivos)
{
    /*
        Se não houverem arquivos, não precisa se ler.
        Poderia tentar abrir o diretório para checagem de erro, mas supondo que esteja
        funcionando corretamente, seria uma perda de tempo tentar abrí-lo.
    */
    if (manipulador_arquivos->n_arquivos == 0) return true;

    char *nomes_arquivos[manipulador_arquivos->n_arquivos];
    unsigned tam_maior_nome_esperado, tam_maior_nome = 64;

    /*
        Este do-while executa até duas vezes.

        Na primeira, tenta-se obter os arquivos do diretório supondo que seus nomes
        têm até 64 caracteres.

        Caso este não seja o caso, na segunda garante-se que haja espaço suficiente para
        que os nomes sejam obtidos.
    */ 
    do
    {
        // Este `if` somente executa na segunda iteração do do-while, para evitar memory leak.
        if (tam_maior_nome > 64)
        {
            for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos; ++i_arquivo)
            {
                free(nomes_arquivos[i_arquivo]);
            }
        }

        /*
            Inicialmente, espera-se que o maior nome seja de 64 caracteres.
            Porém, caso `obter_lista_arquivos` retorne um valor maior,
            `nomes_arquivos` é alocado usando esse valor maior para receber os nomes.
        */
        tam_maior_nome_esperado = tam_maior_nome;

        for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos; ++i_arquivo)
        {
            nomes_arquivos[i_arquivo] = (char *) malloc(tam_maior_nome_esperado * sizeof(char));
        }

        tam_maior_nome = obter_lista_arquivos(manipulador_arquivos, nomes_arquivos, 64);

        // Erro de abertura de diretório.
        if (tam_maior_nome == 0) 
        {
            printf("\nERRO: Falha ao inicializar lista de arquivos. [info_usuario::inicializar_lista_arquivos]\n\n");

            return false;
        }

    } while (tam_maior_nome > tam_maior_nome_esperado);

    unsigned id_arquivo;

    for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos; ++i_arquivo)
    {
        id_arquivo = funcao_conversora(nomes_arquivos[i_arquivo]);

        // Realiza o free na primeira oportunidade possível.
        free(nomes_arquivos[i_arquivo]);

        lista_arquivos[id_arquivo] = VAZIO;
    }

    return true;
}