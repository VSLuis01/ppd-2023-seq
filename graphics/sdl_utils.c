//
// Created by luis on 27/05/23.
//

#include "sdl_utils.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

TTF_Font *fonte;
SDL_Surface *superficieTexto;
SDL_Texture *texturaTexto;

/**
 * @brief Inicializa a janela do SDL (window e renderer)
 * @param width comprimento da janela
 * @param height altura da janela
 */
void sdlInitWindow(int width, int height) {
    // Inicialização do SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    // Criação da janela
    window = SDL_CreateWindow("Exemplo SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

/**
 * @brief Destrói a janela do SDL (renderer e window)
 */
void sdlDestroyWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * @brief Verifica se o evento é de fechar a janela.
 * @return
 */
int sdlEventClosedWindow() {
    return event.type == SDL_QUIT;
}

void sdlClearRender() {
    // Limpa o renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void sdlDraw() {
    // Atualiza a janela
    SDL_RenderPresent(renderer);
}

/**
 * @brief Verifica se há eventos na fila do SDL
 * @return 1 se há eventos, 0 caso contrário.
 */
int sdlPullEvent() {
    return SDL_PollEvent(&event);
}

void destroyTexto() {
    // Limpar recursos
    SDL_FreeSurface(superficieTexto);
    SDL_DestroyTexture(texturaTexto);
    TTF_CloseFont(fonte);
}

void initTexto(const char *texto, int tamanhoFonte) {
    // Renderizar texto
    fonte = TTF_OpenFont("../opensans.ttf", tamanhoFonte);
    SDL_Color corTexto = {255, 255, 255, 255};
    superficieTexto = TTF_RenderText_Solid(fonte, texto, corTexto);
    texturaTexto = SDL_CreateTextureFromSurface(renderer, superficieTexto);
}

/**
 * @brief Renderiza um círculo no renderer
 * @param pos posicao do centro do círculo na janela
 * @param raio raio do circulo
 * @param color cor do circulo
 */
void sdlRenderizarCirculo(SDL_FPoint pos, int raio, SDL_Colour color, const char *label) {
    // Configura a cor do renderer
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int x, y;
    for (y = -raio; y <= raio; y++) {
        for (x = -raio; x <= raio; x++) {
            // Verifica se o ponto está dentro do círculo
            if (x * x + y * y <= raio * raio) {
                // Desenha o ponto no renderer
                SDL_RenderDrawPoint(renderer, pos.x + x, pos.y + y);
            }
        }
    }

    initTexto(label, 12);

    // Definir a posição do label
    int posX = pos.x - superficieTexto->w / 2;
    int posY = pos.y - superficieTexto->h / 2;

    // Renderizar o label
    SDL_Rect destinoTexto = {posX, posY, superficieTexto->w, superficieTexto->h};
    SDL_RenderCopy(renderer, texturaTexto, NULL, &destinoTexto);

    destroyTexto();
}

/**
 * @brief Renderiza uma linha no renderer
 * @param p1 ponto inicial da linha (extremo 1)
 * @param p2 ponto final da linha (extremo 2)
 * @param color cor da linha
 */
void sdlRenderizarLinha(SDL_FPoint p1, SDL_FPoint p2, SDL_Colour color, const char *label) {
    // Configura a cor do renderer
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);


    // Desenha a linha utilizando SDL_RenderDrawLine
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);

    // Calcula a posição central da linha
    float centerX = (p1.x + p2.x) / 2;
    float centerY = (p1.y + p2.y) / 2;

    initTexto(label, 12);

    int textoWidth, textoHeight;
    TTF_SizeText(fonte, label, &textoWidth, &textoHeight);

    // Define o deslocamento horizontal e vertical do texto
    int offsetX = textoWidth / 2;
    int offsetY = textoHeight / 2;

    // Renderiza o texto centralizado com deslocamento
    SDL_Rect destinoTexto = {centerX - offsetX, centerY - offsetY, textoWidth, textoHeight};
    SDL_RenderCopy(renderer, texturaTexto, NULL, &destinoTexto);

    destroyTexto();
}


