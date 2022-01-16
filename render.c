#include "render.h"

boolean iniciar(SDL_Window **janela, SDL_Renderer **renderer)
{
    boolean sucesso = TRUE;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erro ao iniciar o SDL. SDL Error: %s\n", SDL_GetError());
        sucesso = FALSE;
    }
    else
    {
        if (alocar_janela(janela))
        {
            if (alocar_renderer(renderer, *janela))
            {
                SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }

    return sucesso;
}

boolean alocar_janela(SDL_Window **janela)
{
    boolean sucesso = TRUE;
    *janela = SDL_CreateWindow("CHIP8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*janela == NULL)
    {
        printf("Erro na criação da janela. SDL Error: %s\n", SDL_GetError());
        sucesso = FALSE;
    }
    return sucesso;
}

boolean alocar_renderer(SDL_Renderer **renderer, SDL_Window *janela)
{
    boolean sucesso = TRUE;
    *renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL)
    {
        printf("Erro na criação do renderer. SDL Error: %s\n", SDL_GetError());
        sucesso = FALSE;
    }
    return sucesso;
}

void fechar(SDL_Window **janela, SDL_Renderer **renderer, SDL_Texture **textura)
{
    SDL_DestroyTexture(*textura);
    *textura = NULL;

    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*janela);
    *renderer = NULL;
    *janela = NULL;

    SDL_Quit();
}

void retangulo(SDL_Renderer *renderer, int x, int y, int l, int h)
{
    SDL_Rect retangulo = {x, y, l, h};
    SDL_SetRenderDrawColor(renderer, 0x00FF, 0x00FF, 0x00FF, 0x00FF);
    SDL_RenderFillRect(renderer, &retangulo);
}