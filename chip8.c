#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

static unsigned short CHIP8_opcodeBuscar(CHIP8 *chip8);
static void CHIP8_opcodeDecodificar(CHIP8 *chip8);

static void OP_cls(CHIP8 *chip8);
static void OP_rts(CHIP8 *chip8);
static void OP_jump(CHIP8 *chip8);
static void OP_call(CHIP8 *chip8);
static void OP_skipEqual(CHIP8 *chip8);
static void OP_skipNotEqual(CHIP8 *chip8);
static void OP_skipEqualReg(CHIP8 *chip8);
static void OP_skipNotEqualReg(CHIP8 *chip8);
static void OP_setReg(CHIP8 *chip8);
static void OP_addReg(CHIP8 *chip8);
static void OP_setRegReg(CHIP8 *chip8);
static void OP_or(CHIP8 *chip8);
static void OP_and(CHIP8 *chip8);
static void OP_xor(CHIP8 *chip8);
static void OP_addCarry(CHIP8 *chip8);
static void OP_subBorrow(CHIP8 *chip8);
static void OP_rShift(CHIP8 *chip8);
static void OP_lShift(CHIP8 *chip8);
static void OP_rSubBorrow(CHIP8 *chip8);
static void OP_setI(CHIP8 *chip8);
static void OP_jumpPlus(CHIP8 *chip8);
static void OP_andRand(CHIP8 *chip8);
static void OP_draw(CHIP8 *chip8);
static void OP_skipKeyPressed(CHIP8 *chip8);
static void OP_skipKeyNotPressed(CHIP8 *chip8);
static void OP_getDelayTimer(CHIP8 *chip8);
static void OP_getKey(CHIP8 *chip8);
static void OP_setDelayTimer(CHIP8 *chip8);
static void OP_setSoundTimer(CHIP8 *chip8);
static void OP_addI(CHIP8 *chip8);
static void OP_setISprite(CHIP8 *chip8);
static void OP_storeBCD(CHIP8 *chip8);
static void OP_StoreV(CHIP8 *chip8);
static void OP_LoadV(CHIP8 *chip8);

CHIP8 * CHIP8_criar()
{
    CHIP8 *chip8 = (CHIP8 *) malloc(1 * sizeof(CHIP8));
    if (chip8 == NULL)
    {
        printf("Erro ao alocar CHIP8\n");
        exit(-1);
    }
    return chip8;
}

void CHIP8_destruir(CHIP8 **chip8)
{
    if (*chip8 != NULL)
    {
        free(*chip8);
        *chip8 = NULL;
    }
}

void CHIP8_inicializar(CHIP8 *chip8)
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

void CHIP8_carregarROM(CHIP8 *chip8, char *romNome)
{
    if (chip8 != NULL)
    {
        FILE *rom = fopen(romNome, "rb");
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

void CHIP8_ciclo(CHIP8 *chip8)
{
    chip8->opcode = CHIP8_opcodeBuscar(chip8);

    CHIP8_opcodeDecodificar(chip8);

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

unsigned char CHIP8_pegarDrawFlag(CHIP8 *chip8)
{
    return chip8->drawFlag;
}

unsigned char CHIP8_pegarPixelTela(CHIP8 *chip8, int i)
{
    return chip8->tela[i];
}

void CHIP8_definirTecla(CHIP8 *chip8, unsigned char x, int i)
{
    chip8->teclas[i] = x;
}

static unsigned short CHIP8_opcodeBuscar(CHIP8 *chip8)
{
    return chip8->opcode = chip8->memoria[chip8->pc] << 8 | chip8->memoria[chip8->pc + 1];
}

static void CHIP8_opcodeDecodificar(CHIP8 *chip8)
{
    unsigned short opcode = chip8->opcode;

    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    OP_cls(chip8);
                    break;

                case 0x000E:
                    OP_rts(chip8);
                    break;

                default:
                    printf ("Opcode desconhecido: 0x%X\n", opcode);
            }
            break;

        case 0x1000:
            OP_jump(chip8);
            break;

        case 0x2000:
            OP_call(chip8);
            break;

        case 0x3000:
            OP_skipEqual(chip8);
            break;

        case 0x4000:
            OP_skipNotEqual(chip8);
            break;

        case 0x5000:
            OP_skipEqualReg(chip8);
            break;

        case 0x6000:
            OP_setReg(chip8);
            break;

        case 0x7000:
            OP_addReg(chip8);
            break;

        case 0x8000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    OP_setRegReg(chip8);
                    break;

                case 0x0001:
                    OP_or(chip8);
                    break;

                case 0x0002:
                    OP_and(chip8);
                    break;

                case 0x0003:
                    OP_xor(chip8);
                    break;

                case 0x0004:
                    OP_addCarry(chip8);
                    break;

                case 0x0005:
                    OP_subBorrow(chip8);
                    break;

                case 0x0006:
                    OP_rShift(chip8);
                    break;

                case 0x0007:
                    OP_rSubBorrow(chip8);
                    break;

                case 0x000E:
                    OP_lShift(chip8);
                    break;

                default:
                    printf("Opcode desconhecido: 0x%X\n", opcode);
            }
            break;

        case 0x9000:
            OP_skipNotEqualReg(chip8);
            break;

        case 0xA000:
            OP_setI(chip8);
            break;

        case 0xB000:
            OP_jumpPlus(chip8);
            break;

        case 0xC000:
            OP_andRand(chip8);
            break;

        case 0xD000:
            OP_draw(chip8);
            break;

        case 0xE000:
            switch (chip8->opcode & 0x00FF)
            {
                case 0x009E:
                    OP_skipKeyPressed(chip8);
                    break;

                case 0x00A1:
                    OP_skipKeyNotPressed(chip8);
                    break;

                default:
                    printf("Opcode desconhecido: 0x%X\n", opcode);
            }
            break;

        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x0007:
                    OP_getDelayTimer(chip8);
                    break;

                case 0x000A:
                    OP_getKey(chip8);
                    break;

                case 0x0015:
                    OP_setDelayTimer(chip8);
                    break;

                case 0x0018:
                    OP_setSoundTimer(chip8);
                    break;

                case 0x001E:
                    OP_addI(chip8);
                    break;

                case 0x0029:
                    OP_setISprite(chip8);
                    break;

                case 0x0033:
                    OP_storeBCD(chip8);
                    break;

                case 0x0055:
                    OP_StoreV(chip8);
                    break;

                case 0x0065:
                    OP_LoadV(chip8);
                    break;

                default:
                    printf("Opcode desconhecido: 0x%X\n", opcode);
            }
            break;
        default:
            printf("Opcode desconhecido: 0x%X\n", opcode);
    }
}

static void OP_cls(CHIP8 *chip8)
{
    for (int i = 0; i < TELA_TAMANHO; i++)
    {
        chip8->tela[i] = 0;
    }
    chip8->pc += 2;
}

static void OP_rts(CHIP8 *chip8)
{
    chip8->sp--;
    chip8->pc = chip8->stack[chip8->sp];
    chip8->pc += 2;
}

static void OP_jump(CHIP8 *chip8)
{
    chip8->pc = chip8->opcode & 0x0FFF;
}

static void OP_call(CHIP8 *chip8)
{
    chip8->stack[chip8->sp] = chip8->pc;
    chip8->sp++;
    chip8->pc = chip8->opcode & 0x0FFF;
}

static void OP_skipEqual(CHIP8 *chip8)
{
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] == (chip8->opcode & 0x00FF))
    {
        chip8->pc += 2;
    }
    chip8->pc += 2;
}

static void OP_skipNotEqual(CHIP8 *chip8)
{
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] != (chip8->opcode & 0x00FF))
    {
        chip8->pc += 2;
    }
    chip8->pc += 2;
}

static void OP_skipEqualReg(CHIP8 *chip8)
{
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] == chip8->V[(chip8->opcode & 0x00F0) >> 4])
    {
        chip8->pc += 2;
    }
    chip8->pc += 2;
}

static void OP_skipNotEqualReg(CHIP8 *chip8)
{
    if (chip8->V[(chip8->opcode & 0x0F00) >> 8] != chip8->V[(chip8->opcode & 0x00F0) >> 4])
    {
        chip8->pc += 2;
    }
    chip8->pc += 2;
}

static void OP_setReg(CHIP8 *chip8)
{
    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->opcode & 0x00FF;
    chip8->pc += 2;
}

static void OP_addReg(CHIP8 *chip8)
{
    chip8->V[(chip8->opcode & 0x0F00) >> 8] += chip8->opcode & 0x00FF;
    chip8->pc += 2;
}

static void OP_setRegReg(CHIP8 *chip8)
{
    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4];
    chip8->pc += 2;
}

static void OP_or(CHIP8 *chip8)
{
    chip8->V[(chip8->opcode & 0x0F00) >> 8] |= chip8->V[(chip8->opcode & 0x00F0) >> 4];
    chip8->pc += 2;
}

static void OP_and(CHIP8 *chip8)
{
    chip8->V[(chip8->opcode & 0x0F00) >> 8] &= chip8->V[(chip8->opcode & 0x00F0) >> 4];
    chip8->pc += 2;
}

static void OP_xor(CHIP8 *chip8)
{
    chip8->V[(chip8->opcode & 0x0F00) >> 8] ^= chip8->V[(chip8->opcode & 0x00F0) >> 4];
    chip8->pc += 2;
}

static void OP_addCarry(CHIP8 *chip8)
{
    if ((chip8->V[(chip8->opcode & 0x00F0) >> 4]) > (0xFF - chip8->V[(chip8->opcode & 0x0F00) >> 8]))
    {
        chip8->V[0xF] = 1;
    }
    else
    {
        chip8->V[0xF] = 0;
    }
    chip8->V[(chip8->opcode & 0x0F00) >> 8] += chip8->V[(chip8->opcode & 0x00F0) >> 4];
    chip8->pc += 2;
}

static void OP_subBorrow(CHIP8 *chip8)
{
    if ((chip8->V[(chip8->opcode & 0x00F0) >> 4]) > (chip8->V[(chip8->opcode & 0x0F00) >> 8]))
    {
        chip8->V[0xF] = 0;
    }
    else
    {
        chip8->V[0xF] = 1;
    }
    chip8->V[(chip8->opcode & 0x0F00) >> 8] -= chip8->V[(chip8->opcode & 0x00F0) >> 4];
    chip8->pc += 2;
}

static void OP_rShift(CHIP8 *chip8)
{
    chip8->V[0xF] = chip8->V[(chip8->opcode & 0x0F00) >> 8] & 0x1;
    chip8->V[(chip8->opcode & 0x0F00) >> 8] >>= 0x1;
    chip8->pc += 2;
}

static void OP_lShift(CHIP8 *chip8)
{
    chip8->V[0xF] = chip8->V[(chip8->opcode & 0x0F00) >> 8] >> 7;
    chip8->V[(chip8->opcode & 0x0F00) >> 8] <<= 1;
    chip8->pc += 2;
}

static void OP_rSubBorrow(CHIP8 *chip8)
{
    if ((chip8->V[(chip8->opcode & 0x0F00) >> 8]) > (chip8->V[(chip8->opcode & 0x00F0) >> 4]))
    {
        chip8->V[0xF] = 0;
    }
    else
    {
        chip8->V[0xF] = 1;
    }
    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->V[(chip8->opcode & 0x00F0) >> 4] - chip8->V[(chip8->opcode & 0x0F00) >> 8];
    chip8->pc += 2;
}

static void OP_setI(CHIP8 *chip8)
{
    chip8->I = chip8->opcode & 0x0FFF;
    chip8->pc += 2;
}

static void OP_jumpPlus(CHIP8 *chip8)
{
    chip8->pc = chip8->V[0] + (chip8->opcode & 0x0FFF);
}

static void OP_andRand(CHIP8 *chip8)
{
    srand(time(NULL));
    chip8->V[(chip8->opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (chip8->opcode & 0x00FF);
    chip8->pc += 2;
}

static void OP_draw(CHIP8 *chip8)
{
    unsigned short x = chip8->V[(chip8->opcode & 0x0F00) >> 8];
    unsigned short y = chip8->V[(chip8->opcode & 0x00F0) >> 4];
    unsigned short h = chip8->opcode & 0x000F;
    unsigned short pixel;
    chip8->V[0xF] = 0;

    for (int iY = 0; iY < h; iY++)
    {
        pixel = chip8->memoria[chip8->I + iY];
        for (int iX = 0; iX < 8; iX++)
        {
            if ((pixel & (0x80 >> iX)) != 0)
            {
                if (chip8->tela[x + iX + ((y + iY) * 64)] == 1)
                {
                    chip8->V[0xF] = 1;
                }
                chip8->tela[x + iX + ((y + iY) * 64)] ^= 1;
            }
        }
    }
    chip8->drawFlag = 1;
    chip8->pc += 2;
}

static void OP_skipKeyPressed(CHIP8 *chip8)
{
    if (chip8->teclas[chip8->V[(chip8->opcode & 0x0F00) >> 8]] != 0)
    {
        chip8->pc += 2;
    }
    chip8->pc += 2;
}

static void OP_skipKeyNotPressed(CHIP8 *chip8)
{
    if (chip8->teclas[chip8->V[(chip8->opcode & 0x0F00) >> 8]] == 0)
    {
        chip8->pc += 2;
    }
    chip8->pc += 2;
}

static void OP_getDelayTimer(CHIP8 *chip8)
{
    chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->delayTimer;
    chip8->pc += 2;
}

static void OP_getKey(CHIP8 *chip8)
{
    int pressed = 0;
    for (int i = 0; i < TECLAS_QUANTIDADE; i++)
    {
        if (chip8->teclas[i] != 0)
        {
            chip8->V[(chip8->opcode & 0x0F00) >> 8] = i;
            pressed = 1;
        }
    }
    if (!pressed)
    {
        return;
    }
    chip8->pc += 2;
}

static void OP_setDelayTimer(CHIP8 *chip8)
{
    chip8->delayTimer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
    chip8->pc += 2;
}

static void OP_setSoundTimer(CHIP8 *chip8)
{
    chip8->soundTimer = chip8->V[(chip8->opcode & 0x0F00) >> 8];
    chip8->pc += 2;
}

static void OP_addI(CHIP8 *chip8)
{
    chip8->I += chip8->V[(chip8->opcode & 0x0F00) >> 8];
    chip8->pc += 2;
}

static void OP_setISprite(CHIP8 *chip8)
{
    chip8->I = chip8->V[(chip8->opcode & 0x0F00) >> 8] * 0x5;
    chip8->pc += 2;
}


static void OP_storeBCD(CHIP8 *chip8)
{
    chip8->memoria[chip8->I] = chip8->V[(chip8->opcode & 0x0F00) >> 8] / 100;
    chip8->memoria[chip8->I + 1] = (chip8->V[(chip8->opcode & 0x0F00) >> 8] / 10) % 10;
    chip8->memoria[chip8->I + 2] = (chip8->V[(chip8->opcode & 0x0F00) >> 8] % 100) % 10;
    chip8->pc += 2;
}

static void OP_StoreV(CHIP8 *chip8)
{
    for (int i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); i++)
    {
        chip8->memoria[chip8->I + i] = chip8->V[i];
    }
    chip8->I += ((chip8->opcode & 0x0F00) >> 8) + 1;
    chip8->pc += 2;
}

static void OP_LoadV(CHIP8 *chip8)
{
    for (int i = 0; i <= ((chip8->opcode & 0x0F00) >> 8); i++)
    {
        chip8->V[i] = chip8->memoria[chip8->I + i];
    }
    chip8->I += ((chip8->opcode & 0x0F00) >> 8) + 1;
    chip8->pc += 2;
}