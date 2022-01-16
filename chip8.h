#ifndef __CHIP8_H__
#define __CHIP8_H__

#define MEMORIA_TAM 4096
#define STACK_TAMANHO 16
#define REGISTRADORES_QUANTIDADE 16
#define TELA_TAMANHO 64 * 32
#define FONTSET_TAMANHO 80
#define TECLAS_QUANTIDADE 16

typedef struct chip8_st CHIP8;

CHIP8 * CHIP8_criar();
void CHIP8_destruir(CHIP8 **chip8);
void CHIP8_inicializar(CHIP8 *chip8);
void CHIP8_carregarROM(CHIP8 *chip8, char *romNome);
void CHIP8_ciclo(CHIP8 *chip8);
unsigned char CHIP8_pegarPixelTela(CHIP8 *chip8, int i);
unsigned char CHIP8_pegarDrawFlag(CHIP8 *chip8);
void CHIP8_definirTecla(CHIP8 *chip8, unsigned char x, int i);

#endif // __CHIP8_H__