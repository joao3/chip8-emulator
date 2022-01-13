#include <stdlib.h>
#include <stdio.h>

#include "chip8.h"

unsigned char CHIP8_Fontset[FONTSET_TAMANHO] =
{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct chip8_st
{
    unsigned short opcode;
    unsigned char memoria[MEMORIA_TAM];
    unsigned char V[REGISTRADORES_QUANTIDADE];
    unsigned short I;
    unsigned short pc;
    unsigned char tela[TELA_TAMANHO];
    unsigned short stack[STACK_TAMANHO];
    unsigned short sp;
    unsigned char teclas[TECLAS_QUANTIDADE];
    unsigned char drawFlag;
    unsigned char delayTimer;
    unsigned char soundTimer;
};

static unsigned short CHIP8_OpcodeBuscar(CHIP8 *chip8);
static void CHIP8_OpcodeDecodificar(CHIP8 *chip8);

CHIP8 * CHIP8_Criar()
{
    CHIP8 *chip8 = (CHIP8 *) malloc(1 * sizeof(CHIP8));
    if (chip8 == NULL)
    {
        printf("Erro ao alocar CHIP8\n");
        exit(-1);
    }
    return chip8;
}

void CHIP8_Inicializar(CHIP8 *chip8)
{
    if (chip8 != NULL)
    {
        chip8->pc = 0x200;
        chip8->opcode = 0;
        chip8->I = 0;
        chip8-> sp = 0;

        for (int i = 0; i < TELA_TAMANHO; i++)
        {
            chip8->tela[i] = 0;
        }

        for (int i = 0; i < MEMORIA_TAM; i++)
        {
            chip8->memoria[i] = 0;
        }

        for (int i = 0; i < STACK_TAMANHO; i++)
        {
            chip8->stack[i] = 0;
        }

        for (int i = 0; i < REGISTRADORES_QUANTIDADE; i++)
        {
            chip8->V[i] = 0;
        }

        for (int i = 0; i < TECLAS_QUANTIDADE; i++)
        {
            chip8->teclas[i] = 0;
        }

        for (int i = 0; i < FONTSET_TAMANHO; i++)
        {
            chip8->memoria[i] = CHIP8_Fontset[i];
        }

        chip8->delayTimer = 0;
        chip8->soundTimer = 0;
    }
}

void CHIP8_CarregarROM(CHIP8 *chip8, char *romNome)
{
    if (chip8 != NULL)
    {
        FILE *rom = fopen(romNome, "r");
        int i = 0;
        while (!feof(rom))
        {
            char c = fgetc(rom);
            chip8->memoria[i + 512] = c;
            i++;
        }
        fclose(rom);
    }
}

void CHIP8_Ciclo(CHIP8 *chip8)
{
    chip8->opcode = CHIP8_OpcodeBuscar(chip8);

    CHIP8_OpcodeDecodificar(chip8);

    if (chip8->delayTimer > 0)
    {
        chip8->delayTimer--;
    }
    if (chip8->soundTimer > 0)
    {
        if (chip8->soundTimer == 1)
        {
            printf("BEEP\n");
        }
        chip8->soundTimer--;
    }
}

static unsigned short CHIP8_OpcodeBuscar(CHIP8 *chip8)
{
    if (chip8 != NULL)
    {
        return chip8->opcode = chip8->memoria[chip8->pc] << 8 | chip8->memoria[chip8->pc + 1];
    }
}

static void CHIP8_OpcodeDecodificar(CHIP8 *chip8)
{

}