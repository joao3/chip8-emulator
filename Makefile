# Arquivos .c
OBJS = main.c chip8.c render.c

# Compilador
CC = gcc

# Flags de compilação
COMPILER_FLAGS = -Wall -march=native -O3

ifeq ($(OS),Windows_NT)
	SDL2_INCLUDE = -Ilib/SDL2/include
	SDL2_LIB = -LD:./lib/SDL2/lib
	SDL2_LINKER = -lmingw32 -lSDL2main -lSDL2
endif

# Executável
OBJ_NAME = main

# Compilar
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(SDL2_INCLUDE) $(SDL2_LIB) $(SDL2_LINKER) -o $(OBJ_NAME)