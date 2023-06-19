#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "info_usuario.h"

void construir_info_usuario(info_usuario_t *informacoes_usuario, const unsigned id, const unsigned n_arquivos)
{
    informacoes_usuario->id_usuario = id;

    construir_info_arquivos(&informacoes_usuario->info_arquivos, n_arquivos);

    inicializar_dado_concorrente(&informacoes_usuario->controlador_info_arquivos, &informacoes_usuario->info_arquivos);
}

void construir_info_arquivos(info_arquivos_t *info_arquivos, const unsigned n_arquivos)
{
    info_arquivos->n_arquivos = n_arquivos;
    info_arquivos->n_vazios = n_arquivos;
    info_arquivos->n_em_progresso = 0;
    info_arquivos->n_completos = 0;

    info_arquivos->estado_arquivos = (estado_progresso_t *) calloc(n_arquivos, sizeof(estado_progresso_t));
}

bool inicializar_info_arquivos(info_arquivos_t *info_arquivos, unsigned (funcao_conversora) (const char[]), manipulador_arquivos_t *manipulador_arquivos)
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
            printf("\nERRO: Falha ao inicializar estados de arquivos. [info_usuario::inicializar_info_arquivos]\n\n");

            return false;
        }

    } while (tam_maior_nome > tam_maior_nome_esperado);

    #ifdef DEBUG
    {
        char dbg_para_printar[manipulador_arquivos->n_arquivos * tam_maior_nome + 128];
        snprintf(dbg_para_printar, 128, "\nDEBUG: %u arquivos inicialmente no diretorio %s\n", manipulador_arquivos->n_arquivos, manipulador_arquivos->nome_diretorio);
        for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos; ++i_arquivo)
        {
            strcat(dbg_para_printar, nomes_arquivos[i_arquivo]);
            strcat(dbg_para_printar, "\n");
        }
        printf("%s\n", dbg_para_printar);
    }
    #endif

    unsigned id_arquivo;

    // Preenche a lista de arquivos deste usuário marcando os arquivos já presentes em seu diretório.
    for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos; ++i_arquivo)
    {
        id_arquivo = funcao_conversora(nomes_arquivos[i_arquivo]);

        // Realiza o free na primeira oportunidade possível.
        free(nomes_arquivos[i_arquivo]);

        info_arquivos->estado_arquivos[id_arquivo] = COMPLETO;
        --info_arquivos->n_vazios;
        ++info_arquivos->n_completos;
    }

    return true;
}

void abrir_info_arquivos(dado_concorrente_t *controlador_info_arquivos)
{
    pthread_mutex_lock(&controlador_info_arquivos->mutex_mensagem);
}

void fechar_info_arquivos(dado_concorrente_t *controlador_info_arquivos)
{
    pthread_mutex_unlock(&controlador_info_arquivos->mutex_mensagem);
}

void obter_arquivos_em_progresso(const dado_concorrente_t *controlador_info_arquivos, unsigned arquivos_em_progresso[])
{
    const info_arquivos_t *informacoes_arquivos = (const info_arquivos_t *) controlador_info_arquivos->dado;

    for (unsigned i_arquivo = 0, i_em_progresso = 0; i_arquivo < informacoes_arquivos->n_arquivos; ++i_arquivo)
    {
        estado_progresso_t estado_arquivo = informacoes_arquivos->estado_arquivos[i_arquivo];

        if (estado_arquivo == EM_PROGRESSO)
        {
            arquivos_em_progresso[i_em_progresso] = i_arquivo;
            ++i_em_progresso;
        }
    }
}