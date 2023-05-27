//
// Created by luis on 27/05/23.
//

#ifndef PPD_2023_SEQ_SDL_UTILS_H
#define PPD_2023_SEQ_SDL_UTILS_H

#include <SDL2/SDL.h>
#include <math.h>

typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Color;

int sdlInitWindow(int width, int height);

void sdlDestroyWindow();

int sdlQuit();

void sdlRender();

int sdlPullEvent();

void renderizarCirculoPreenchido(int centroX, int centroY, int raio, Color color);

#endif //PPD_2023_SEQ_SDL_UTILS_H
