#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL2/SDL.h>

typedef int boolean;
#define FALSE 0
#define TRUE 1

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

boolean iniciar(SDL_Window **janela, SDL_Renderer **renderer);
boolean alocar_janela(SDL_Window **janela);
boolean alocar_renderer(SDL_Renderer **renderer, SDL_Window *janela);
void fechar(SDL_Window **janela, SDL_Renderer **renderer, SDL_Texture **textura);
void retangulo(SDL_Renderer *renderer, int x, int y, int l, int h);

#endif // __RENDER_H__