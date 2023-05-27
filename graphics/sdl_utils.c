//
// Created by luis on 27/05/23.
//

#include "sdl_utils.h"



SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

void sdlDestroyWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int sdlQuit() {
    return event.type == SDL_QUIT;
}

int sdlPullEvent() {
    return SDL_PollEvent(&event);
}

void sdlRender() {
    // Limpa o renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Atualiza a janela
    SDL_RenderPresent(renderer);
}

int sdlInitWindow(int width, int height) {
    // Inicialização do SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Criação da janela
    window = SDL_CreateWindow("Exemplo SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

}

void renderizarCirculoPreenchido(int centroX, int centroY, int raio, Color color) {
    int x, y, yAnterior;

    // Percorre cada linha horizontal do círculo
    for (y = centroY - raio; y <= centroY + raio; y++) {
        // Calcula as coordenadas x para a linha atual
        x = centroX + sqrt(raio * raio - (y - centroY) * (y - centroY));

        // Desenha a linha horizontal
        SDL_RenderDrawLine(renderer, centroX - x, y, centroX + x, y);
    }
}
