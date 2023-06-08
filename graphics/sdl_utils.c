//
// Created by luis on 27/05/23.
//

#include "sdl_utils.h"

#define FONT_SIZE 12

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

TTF_Font *fonte;
SDL_Surface *superficieTexto;
SDL_Texture *texturaTexto;

typedef struct {
    SDL_Point pos;
} PosicoesVertices;

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
    window = SDL_CreateWindow("Arvore Geradora Mínima Sequencial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
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
void sdlRenderizarCirculo(SDL_Point pos, int raio, SDL_Colour color, const char *label) {
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

    initTexto(label, FONT_SIZE);

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
void sdlRenderizarLinha(SDL_Point p1, SDL_Point p2, SDL_Colour color, const char *label) {
    // Configura a cor do renderer
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);


    // Desenha a linha utilizando SDL_RenderDrawLine
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);

    // Calcula a posição central da linha
    float centerX = (p1.x + p2.x) / 2;
    float centerY = (p1.y + p2.y) / 2;

    initTexto(label, FONT_SIZE);

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

void desenharGrafo(Grafo* grafo) {
    // Determine a largura e altura da janela
    int larguraJanela, alturaJanela;
    int raioVertice = 10;
    SDL_GetRendererOutputSize(renderer, &larguraJanela, &alturaJanela);

    PosicoesVertices vertices[grafo->V];

    // Calcule o raio máximo permitido com base nas dimensões da janela
    int raioMaximo = (larguraJanela < alturaJanela ? larguraJanela : alturaJanela) / 3;
    srand(5);

    // Calcule as coordenadas do centro da janela
    int centerX = larguraJanela / 2;
    int centerY = alturaJanela / 2;

    // Desenhe os vértices em um círculo em torno do centro da janela
    for (int i = 0; i < grafo->V; i++) {
        // Gere um raio aleatório para o vértice atual
        int raio = rand() % raioMaximo + 200;

        // Calcule o ângulo de separação entre os vértices
        double anguloSeparacao = 2 * M_PI / grafo->V;

        // Calcule o ângulo atual para o vértice atual
        double angulo = grafo->vertices[i].v * anguloSeparacao;
        char label[10];
        sprintf(label, "%d", grafo->vertices[i].v);

        // Calcule as coordenadas do vértice com base no raio e no ângulo
        int posX = centerX + raio * cos(angulo);
        int posY = centerY + raio * sin(angulo);

        vertices[grafo->vertices[i].v - 1].pos = (SDL_Point){posX, posY};

        // Desenhe o vértice na posição calculada
        sdlRenderizarCirculo((SDL_Point){posX, posY}, raioVertice, (SDL_Colour){255, 0, 0, 255}, label);
    }

    // Renderiza as arestas
    for (int i = 0; i < grafo->A; ++i) {
        char label[10];
        sprintf(label, "%d", grafo->arestas[i].peso);
        sdlRenderizarLinha(vertices[grafo->arestas[i].v - 1].pos, vertices[grafo->arestas[i].w - 1].pos, (SDL_Colour){0, 0, 255, 255}, label);
    }

}


void renderizarGrafo(Grafo *grafo, RenderFunction renderFunction) {
    // Configurações do layout do grafo
    // Chamar a função de renderização específica
    renderFunction(grafo);
}


