CXX = gcc
CXX_FLAGS_ALL = -Wall -Wextra -pedantic -std=c11 -O3 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fno-sanitize-recover -fstack-protector -g
CXX_FLAGS = $(CXX_FLAGS_MAIN) -c

all: main.o usuario.o info_compartilhada.o
	@$(CXX) $(CXX_FLAGS_ALL) main.o info_compartilhada.o usuario.o -o simulador.exe

main.o: main.c
	@$(CXX) $(CXX_FLAGS) main.c

usuario.o: usuario.c usuario.h
	@$(CXX) $(CXX_FLAGS) usuario.c

info_compartilhada.o: info_compartilhada.c info_compartilhada.h
	@$(CXX) $(CXX_FLAGS) info_compartilhada.c

clean:
	@rm *.o *.exe *.txt