# Variáveis
COMPILADOR_C = gcc
COMPILADOR_CPP = g++

VERSAO_C = -std=c17
VERSAO_CPP = -std=c++20

FLAGS_C = -Wstrict-prototypes
FLAGS_CPP =

FLAGS_BASICAS = -Wall -Wextra -pedantic -O3

FLAGS_REGULARES = -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wdouble-promotion -Wduplicated-cond -Wfloat-equal -Wformat=2 -Wformat-overflow -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat-truncation -Winit-self -Wlogical-op -Wnonnull-compare -Wpointer-arith -Wshadow -Wshift-overflow=2 -Wstrict-overflow=5 -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wwrite-strings

FLAGS_EXTRAS = -g -D_GLIBCOMPILADOR_DEBUG -D_GLIBCOMPILADOR_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fno-sanitize-recover -fstack-protector

FLAGS = $(FLAGS_BASICAS) $(FLAGS_REGULARES) $(FLAGS_EXTRAS)

TODOS_PONTO_O = util.o lista_encadeada.o lista_mensagem.o buffer.o manipulador_arquivos.o info_compartilhada.o info_usuario.o info_total.o gerenciar_buffers.o solicitar_arquivos.o processamento_mensagens.o par_usuario_arquivo.o usuario.o main.o

# Regras

all: COMPILADOR = $(COMPILADOR_C)
all: FLAGS_BASICAS += $(VERSAO_C)
all: FLAGS += -DDEBUG=0 $(FLAGS_C)
all: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg: COMPILADOR = $(COMPILADOR_C)
dbg: FLAGS_BASICAS += $(VERSAO_C)
dbg: FLAGS += -DDEBUG=3 $(FLAGS_C)
dbg: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg1: COMPILADOR = $(COMPILADOR_C)
dbg1: FLAGS_BASICAS += $(VERSAO_C)
dbg1: FLAGS += -DDEBUG=1 $(FLAGS_C)
dbg1: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg2: COMPILADOR = $(COMPILADOR_C)
dbg2: FLAGS_BASICAS += $(VERSAO_C)
dbg2: FLAGS += -DDEBUG=2 $(FLAGS_C)
dbg2: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg3: COMPILADOR = $(COMPILADOR_C)
dbg3: FLAGS_BASICAS += $(VERSAO_C)
dbg3: FLAGS += -DDEBUG=3 $(FLAGS_C)
dbg3: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg4: COMPILADOR = $(COMPILADOR_C)
dbg4: FLAGS_BASICAS += $(VERSAO_C)
dbg4: FLAGS += -DDEBUG=4 $(FLAGS_C)
dbg4: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg5: COMPILADOR = $(COMPILADOR_C)
dbg5: FLAGS_BASICAS += $(VERSAO_C)
dbg5: FLAGS += -DDEBUG=5 $(FLAGS_C)
dbg5: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg6: COMPILADOR = $(COMPILADOR_C)
dbg6: FLAGS_BASICAS += $(VERSAO_C)
dbg6: FLAGS += -DDEBUG=6 $(FLAGS_C)
dbg6: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg7: COMPILADOR = $(COMPILADOR_C)
dbg7: FLAGS_BASICAS += $(VERSAO_C)
dbg7: FLAGS += -DDEBUG=7 $(FLAGS_C)
dbg7: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe
	
teste: COMPILADOR = $(COMPILADOR_CPP) 
teste: FLAGS_BASICAS += $(VERSAO_CPP)
teste: FLAGS += -DDEBUG=5 $(FLAGS_CPP)
teste: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe
	$(MAKE) clean

main.o: main.c
	$(COMPILADOR) $(FLAGS) -c main.c

usuario.o: usuario.c usuario.h
	$(COMPILADOR) $(FLAGS) -c usuario.c

par_usuario_arquivo.o: par_usuario_arquivo.c par_usuario_arquivo.h
	$(COMPILADOR) $(FLAGS) -c par_usuario_arquivo.c

processamento_mensagens.o: processamento_mensagens.c processamento_mensagens.h
	$(COMPILADOR) $(FLAGS) -c processamento_mensagens.c

solicitar_arquivos.o: solicitar_arquivos.c solicitar_arquivos.h
	$(COMPILADOR) $(FLAGS) -c solicitar_arquivos.c

gerenciar_buffers.o: gerenciar_buffers.c gerenciar_buffers.h
	$(COMPILADOR) $(FLAGS) -c gerenciar_buffers.c

info_total.o: info_total.c info_total.h
	$(COMPILADOR) $(FLAGS) -c info_total.c

info_usuario.o: info_usuario.c info_usuario.h
	$(COMPILADOR) $(FLAGS) -c info_usuario.c

info_compartilhada.o: info_compartilhada.c info_compartilhada.h
	$(COMPILADOR) $(FLAGS) -c info_compartilhada.c

buffer.o: buffer.c buffer.h
	$(COMPILADOR) $(FLAGS) -c buffer.c

lista_mensagem.o: lista_mensagem.c lista_mensagem.h
	$(COMPILADOR) $(FLAGS) -c lista_mensagem.c

lista_encadeada.o: lista_encadeada.c lista_encadeada.h
	$(COMPILADOR) $(FLAGS) -c lista_encadeada.c

manipulador_arquivos.o: manipulador_arquivos.c manipulador_arquivos.h
	$(COMPILADOR) $(FLAGS) -c manipulador_arquivos.c

util.o: util.c util.h
	$(COMPILADOR) $(FLAGS) -c util.c

clean:
	rm *.o *.exe