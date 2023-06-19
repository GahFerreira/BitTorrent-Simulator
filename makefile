# Variáveis
COMPILADOR_C = gcc
COMPILADOR_CPP = g++

VERSAO_C = -std=c99
VERSAO_CPP = -std=c++20

FLAGS_C = -Wstrict-prototypes
FLAGS_CPP =

FLAGS_BASICAS = -Wall -Wextra -pedantic -O3

FLAGS_REGULARES = -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wduplicated-cond -Wfloat-equal -Wformat=2 -Winit-self -Wlogical-op -Wpointer-arith -Wshadow -Wshift-overflow=2 -Wstrict-overflow=5 -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wwrite-strings

FLAGS_EXTRAS = -g -D_GLIBCOMPILADOR_DEBUG -D_GLIBCOMPILADOR_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fno-sanitize-recover -fstack-protector

FLAGS = $(FLAGS_BASICAS) $(FLAGS_REGULARES) $(FLAGS_EXTRAS)

TODOS_PONTO_O = util.o dado_concorrente.o lista_encadeada.o lista_mensagem.o manipulador_arquivos.o info_compartilhada.o info_usuario.o info_total.o processamento_mensagens.o usuario.o main.o

# Regras

all: COMPILADOR = $(COMPILADOR_C)
all: FLAGS_BASICAS += $(VERSAO_C)
all: FLAGS += $(FLAGS_C)
all: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe

dbg: COMPILADOR = $(COMPILADOR_C)
dbg: FLAGS_BASICAS += $(VERSAO_C)
dbg: FLAGS += -DDEBUG $(FLAGS_C)
dbg: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe
	
teste: COMPILADOR = $(COMPILADOR_CPP) 
teste: FLAGS_BASICAS += $(VERSAO_CPP)
teste: FLAGS += $(FLAGS_CPP)
teste: $(TODOS_PONTO_O)
	$(COMPILADOR) $(FLAGS) $(TODOS_PONTO_O) -o simulador.exe
	$(MAKE) clean

main.o: main.c
	$(COMPILADOR) $(FLAGS) -c main.c

usuario.o: usuario.c usuario.h
	$(COMPILADOR) $(FLAGS) -c usuario.c

processamento_mensagens.o: processamento_mensagens.c processamento_mensagens.h
	$(COMPILADOR) $(FLAGS) -c processamento_mensagens.c

info_total.o: info_total.c info_total.h
	$(COMPILADOR) $(FLAGS) -c info_total.c

info_usuario.o: info_usuario.c info_usuario.h
	$(COMPILADOR) $(FLAGS) -c info_usuario.c

info_compartilhada.o: info_compartilhada.c info_compartilhada.h
	$(COMPILADOR) $(FLAGS) -c info_compartilhada.c

lista_mensagem.o: lista_mensagem.c lista_mensagem.h
	$(COMPILADOR) $(FLAGS) -c lista_mensagem.c

lista_encadeada.o: lista_encadeada.c lista_encadeada.h
	$(COMPILADOR) $(FLAGS) -c lista_encadeada.c

manipulador_arquivos.o: manipulador_arquivos.c manipulador_arquivos.h
	$(COMPILADOR) $(FLAGS) -c manipulador_arquivos.c

dado_concorrente.o: dado_concorrente.c dado_concorrente.h
	$(COMPILADOR) $(FLAGS) -c dado_concorrente.c

util.o: util.c util.h
	$(COMPILADOR) $(FLAGS) -c util.c

clean:
	rm *.o *.exe