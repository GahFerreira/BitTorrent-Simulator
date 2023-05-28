CXX = gcc
CXX_FLAGS_ALL = -Wall -Wextra -pedantic -std=c11 -O3 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fno-sanitize-recover -fstack-protector -g
CXX_FLAGS = $(CXX_FLAGS_ALL) -c

all: main.o usuario.o info_usuario.o info_compartilhada.o lista_mensagem.o lista_encadeada.o manipulador_arquivos.o util.o
	$(CXX) $(CXX_FLAGS_ALL) main.o usuario.o info_usuario.o info_compartilhada.o lista_mensagem.o lista_encadeada.o manipulador_arquivos.o util.o -o simulador.exe

main.o: main.c
	$(CXX) $(CXX_FLAGS) main.c

usuario.o: usuario.c usuario.h
	$(CXX) $(CXX_FLAGS) usuario.c

info_usuario.o: info_usuario.c info_usuario.h
	$(CXX) $(CXX_FLAGS) info_usuario.c

info_compartilhada.o: info_compartilhada.c info_compartilhada.h
	$(CXX) $(CXX_FLAGS) info_compartilhada.c

lista_mensagem.o: lista_mensagem.c lista_mensagem.h
	$(CXX) $(CXX_FLAGS) lista_mensagem.c

lista_encadeada.o: lista_encadeada.c lista_encadeada.h
	$(CXX) $(CXX_FLAGS) lista_encadeada.c

manipulador_arquivos.o: manipulador_arquivos.c manipulador_arquivos.h
	$(CXX) $(CXX_FLAGS) manipulador_arquivos.c

util.o: util.c util.h
	$(CXX) $(CXX_FLAGS) util.c

clean:
	rm *.o *.exe *.txt