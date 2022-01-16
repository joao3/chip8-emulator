#include <time.h>

#include "chip8.h"
#include "render.h"

int teclas[TECLAS_QUANTIDADE] =
{
    SDLK_x, SDLK_1, SDLK_2, SDLK_3,
    SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c,
    SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

void imprimir_tela(CHIP8 *chip8, SDL_Renderer *renderer);
void delay(int sec);

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

            if (evento.type == SDL_KEYDOWN)
            {
                int tecla = evento.key.keysym.sym;
                if (tecla == SDLK_ESCAPE)
                {
                    sair = TRUE;
                    break;
                }
                for (int i = 0; i < TECLAS_QUANTIDADE; i++)
                {
                    if (tecla == teclas[i])
                    {
                        CHIP8_definirTecla(chip8, 1, i);
                    }
                }
            }
            if (evento.type == SDL_KEYUP)
            {
                int tecla = evento.key.keysym.sym;
                for (int i = 0; i < TECLAS_QUANTIDADE; i++)
                {
                    if (tecla == teclas[i])
                    {
                        CHIP8_definirTecla(chip8, 0, i);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        CHIP8_ciclo(chip8);

        if (CHIP8_pegarDrawFlag(chip8)) {
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

void delay(int sec)
{
    int m_sec = 1000 * sec;
    clock_t start_time = clock();
    while (clock() < start_time + m_sec);
}