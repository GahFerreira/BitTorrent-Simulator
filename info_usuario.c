#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "info_usuario.h"
#include "par_usuario_arquivo.h"
#include "util.h"

bool inicializar_info_usuario(info_usuario_t *info_usuario, const unsigned id_usuario, const unsigned n_arquivos, const unsigned max_caracteres_dir_usuario)
{
    info_usuario->id_usuario = id_usuario;

    inicializar_lista_mensagem(&info_usuario->lista_tarefas);

    // Novo escopo para desalocar `nome_diretorio` assim que possível.
    {
        // Obtém o nome do diretório para inicializar o manipulador de arquivos.
        char nome_diretorio[max_caracteres_dir_usuario];
        id_usuario_para_nome_diretorio(nome_diretorio, id_usuario);

        inicializar_manipulador_arquivos(&info_usuario->manipulador_arquivos, nome_diretorio, max_caracteres_dir_usuario);
    }

    if ( inicializar_info_arquivos(&info_usuario->info_arquivos, &info_usuario->manipulador_arquivos, n_arquivos) == false )
    {
        printf("[[ERRO]] Falha em inicializar info_arquivos do usuario %u.[info_usuario::inicializar_info_usuario]\n\n", id_usuario+1);

        return false;
    }

    return true;
}

bool inicializar_info_arquivos(info_arquivos_t *info_arquivos, manipulador_arquivos_t *manipulador_arquivos, const unsigned n_arquivos)
{
    info_arquivos->n_arquivos = n_arquivos;
    info_arquivos->n_vazios = n_arquivos;
    info_arquivos->n_em_progresso = 0;
    info_arquivos->n_completos = 0;

    info_arquivos->estado_arquivos = (estado_progresso_t *) calloc(n_arquivos, sizeof(estado_progresso_t));

    if (!inicializar_estado_arquivos(info_arquivos, nome_arquivo_para_id, manipulador_arquivos))
    {
        printf("[[ERRO]] Falha em inicializar estados dos arquivos do diretorio %s. [info_usuario::inicializar_info_arquivos]\n\n", manipulador_arquivos->nome_diretorio);

        return false;
    }

    info_arquivos->mutex_info_arquivos = PTHREAD_MUTEX_INITIALIZER;

    return true;
}

bool inicializar_estado_arquivos(info_arquivos_t *info_arquivos, unsigned (funcao_conversora) (const char[]), manipulador_arquivos_t *manipulador_arquivos)
{
    /*
        Se não houverem arquivos, não precisa se ler.
        Poderia tentar abrir o diretório para checagem de erro, mas supondo que esteja
        funcionando corretamente, seria uma perda de tempo tentar abrí-lo.
    */
    if (manipulador_arquivos->n_arquivos_diretorio == 0) return true;

    char *nomes_arquivos[manipulador_arquivos->n_arquivos_diretorio];
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
            for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos_diretorio; ++i_arquivo)
            {
                free(nomes_arquivos[i_arquivo]);
            }
        }

        /*
            Inicialmente, espera-se que o maior nome seja de 64 caracteres.
            Porém, caso `obter_nomes_arquivos_diretorio` retorne um valor maior,
            `nomes_arquivos` é alocado usando esse valor maior para receber os nomes.
        */
        tam_maior_nome_esperado = tam_maior_nome;

        for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos_diretorio; ++i_arquivo)
        {
            nomes_arquivos[i_arquivo] = (char *) malloc(tam_maior_nome_esperado * sizeof(char));
        }

        tam_maior_nome = obter_nomes_arquivos_diretorio(manipulador_arquivos, nomes_arquivos, 64);

        // Erro de abertura de diretório.
        if (tam_maior_nome == 0) 
        {
            printf("[[ERRO]] Falha ao inicializar estados de arquivos. [info_usuario::inicializar_estado_arquivos]\n\n");

            return false;
        }

    } while (tam_maior_nome > tam_maior_nome_esperado);

    #if DEBUG >= 1
    {
        char dbg_para_printar[manipulador_arquivos->n_arquivos_diretorio * tam_maior_nome + 128];
        snprintf(dbg_para_printar, 128, "\n[DEBUG-1] %u arquivos inicialmente no diretorio %s\n", manipulador_arquivos->n_arquivos_diretorio, manipulador_arquivos->nome_diretorio);
        for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos_diretorio; ++i_arquivo)
        {
            strcat(dbg_para_printar, nomes_arquivos[i_arquivo]);
            strcat(dbg_para_printar, "\n");
        }
        printf("%s\n", dbg_para_printar);
    }
    #endif

    unsigned id_arquivo;

    // Preenche a lista de arquivos deste usuário marcando os arquivos já presentes em seu diretório.
    for (unsigned i_arquivo = 0; i_arquivo < manipulador_arquivos->n_arquivos_diretorio; ++i_arquivo)
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

void trancar_info_arquivos(info_arquivos_t *info_arquivos)
{
    pthread_mutex_lock(&info_arquivos->mutex_info_arquivos);
}

void destrancar_info_arquivos(info_arquivos_t *info_arquivos)
{
    pthread_mutex_unlock(&info_arquivos->mutex_info_arquivos);
}

void obter_arquivos_generico(info_arquivos_t *info_arquivos, unsigned vetor_arquivos[], estado_progresso_t estado_escolhido)
{
    for (unsigned i_arquivo = 0, i_vetor_arquivos = 0; i_arquivo < info_arquivos->n_arquivos; ++i_arquivo)
    {
        estado_progresso_t estado_arquivo = info_arquivos->estado_arquivos[i_arquivo];

        if (estado_arquivo == estado_escolhido)
        {
            vetor_arquivos[i_vetor_arquivos] = i_arquivo;
            ++i_vetor_arquivos;
        }
    }
}

void obter_arquivos_ausentes(info_arquivos_t *info_arquivos, unsigned arquivos_faltantes[])
{
    obter_arquivos_generico(info_arquivos, arquivos_faltantes, VAZIO);
}

void obter_arquivos_em_progresso(info_arquivos_t *info_arquivos, unsigned arquivos_em_progresso[])
{
    obter_arquivos_generico(info_arquivos, arquivos_em_progresso, EM_PROGRESSO);
}

void obter_arquivos_completos(info_arquivos_t *info_arquivos, unsigned arquivos_completos[])
{
    obter_arquivos_generico(info_arquivos, arquivos_completos, COMPLETO);
}

estado_progresso_t obter_estado_arquivo(info_arquivos_t *info_arquivos, const unsigned id_arquivo)
{
    estado_progresso_t resultado;

    if (id_arquivo >= info_arquivos->n_arquivos)
    {
        printf("[[ERRO]] Tentativa de obter estado de arquivo invalido. | Num. arq. disponiveis: %u | id requisitado: %u [info_usuario::obter_estado_arquivo]\n\n", info_arquivos->n_arquivos, id_arquivo+1);

        resultado = VAZIO;
    }

    else
    {
        resultado = info_arquivos->estado_arquivos[id_arquivo];
    }

    return resultado;
}

bool mudar_arquivo_para_em_progresso(info_arquivos_t *info_arquivos, const unsigned id_usuario, const unsigned id_arquivo)
{
    if (obter_estado_arquivo(info_arquivos, id_arquivo) != VAZIO)
    {
        printf("[[ERRO]] Tentativa de mudar arquivo %u do usuario %u para EM_PROGRESSO, mas o arquivo nao esta VAZIO. [info_usuario::mudar_arquivo_para_em_progresso]\n\n", id_arquivo+1, id_usuario+1);

        return false;
    }

    trancar_info_arquivos(info_arquivos);

    --info_arquivos->n_vazios;
    ++info_arquivos->n_em_progresso;

    info_arquivos->estado_arquivos[id_arquivo] = EM_PROGRESSO;

    destrancar_info_arquivos(info_arquivos);

    return true;
}

// bool arquivo_para_completo();

void adicionar_tarefa(lista_mensagem_t *lista_tarefa, const unsigned id_usuario, const unsigned id_usuario_tarefa, const unsigned id_arquivo_tarefa)
{
    if (id_usuario == id_usuario_tarefa)
    {
        printf("[[ERRO]] Tentativa de adicionar tarefa a sua propria lista de tarefas pelo usuario %u. [info_usuario::adicionar_tarefa]\n\n", id_usuario+1);

        return;
    }

    par_usuario_arquivo_t *tarefa = (par_usuario_arquivo_t *) malloc(sizeof(par_usuario_arquivo_t));

    tarefa->id_usuario = id_usuario_tarefa;
    tarefa->id_arquivo = id_arquivo_tarefa;

    #if DEBUG >= 2
    printf("[DEBUG-2] Usuario %u adiciona tarefa: <usuario: %u, arquivo: %u>\n\n", id_usuario+1, id_usuario_tarefa+1, id_arquivo_tarefa+1);
    #endif

    adicionar_elemento_lista_mensagem(lista_tarefa, tarefa);
}

void completar_tarefa(lista_mensagem_t *lista_tarefa, const unsigned id_usuario, const unsigned id_usuario_tarefa, const unsigned id_arquivo_tarefa)
{
    if (id_usuario == id_usuario_tarefa)
    {
        printf("[[ERRO]] Tentativa de completar tarefa da sua propria lista de tarefas pelo usuario %u. [info_usuario::completar_tarefa]", id_usuario+1);

        return;
    }

    par_usuario_arquivo_t tarefa;

    tarefa.id_usuario = id_usuario_tarefa;
    tarefa.id_arquivo = id_arquivo_tarefa;

    #if DEBUG >= 2
    printf("[DEBUG-2] Usuario %u completa tarefa: <usuario: %u, arquivo: %u>\n\n", id_usuario+1, id_usuario_tarefa+1, id_arquivo_tarefa+1);
    #endif

    const par_usuario_arquivo_t *tarefa_a_completar = (const par_usuario_arquivo_t *) extrair_elemento_lista_mensagem(lista_tarefa, &tarefa, (bool (*) (const void *, const void *)) comparar_par_usuario_arquivo);

    if (tarefa_a_completar == NULL)
    {
        printf("[[ERRO]] Falha em extrair a tarefa <id_usuario: %u, arquivo: %u> da lista de tarefas do usuario %u. [info_usuario.c::completar_tarefa]\n\n", id_usuario_tarefa+1, id_arquivo_tarefa+1, id_usuario+1);
    }

    else free((par_usuario_arquivo_t *) tarefa_a_completar);
}