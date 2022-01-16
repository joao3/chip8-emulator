#include <time.h>

#include "chip8.h"
#include "render.h"

void delay(int sec)
{
    int m_sec = 1000 * sec;
    clock_t start_time = clock();
    while (clock() < start_time + m_sec);
}

void imprimir_tela(CHIP8 *chip8, SDL_Renderer *renderer);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s rom.ch8", argv[0]);
        exit(-1);
    }

    CHIP8 *chip8 = CHIP8_criar();
    CHIP8_inicializar(chip8);
    CHIP8_carregarROM(chip8, argv[1]);


    SDL_Window *janela = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *textura = NULL;

    iniciar(&janela, &renderer);

    boolean sair = FALSE;
    SDL_Event evento;

    while (!sair)
    {
        while(SDL_PollEvent(&evento) != 0)
        {
            if (evento.type == SDL_QUIT)
            {
                sair = TRUE;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        CHIP8_ciclo(chip8);

        if (CHIP8_pegarDrawFlag(chip8))
        {
            imprimir_tela(chip8, renderer);
        }


        SDL_RenderPresent(renderer);

        delay(1);
    }

    fechar(&janela, &renderer, &textura);
    CHIP8_destruir(&chip8);

    return 0;
}

void imprimir_tela(CHIP8 *chip8, SDL_Renderer *renderer)
{
    for (int i = 0; i < TELA_TAMANHO; i++)
    {
        if (CHIP8_pegarPixelTela(chip8, i))
        {
            int x = i % 64;
            int y = i / 64;
            retangulo(renderer, x * 10, y * 10, 10, 10);
        }
    }
}