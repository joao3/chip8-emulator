# Arquivos .c
OBJS = main.c chip8.c render.c

# Compilador
CC = gcc

# Flags de compilação
COMPILER_FLAGS = -Wall -march=native -O3

# Flags do SDL
LINKER_FLAGS = -lSDL2

# Executável
OBJ_NAME = main

# Compilar
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)