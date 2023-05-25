CXX = gcc
CXX_FLAGS_MAIN = -Wall -Wextra -pedantic -std=c11 -O3 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fno-sanitize-recover -fstack-protector -g
CXX_FLAGS = $(CXX_FLAGS_MAIN) -c

all: main.o
	@$(CXX) $(CXX_FLAGS_MAIN) main.o -o simulador.exe

main.o: main.c
	$(CXX) $(CXX_FLAGS) main.c

clean:
	@rm *.o *.exe *.txt