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

void renderizarGrafoHierarquico(Grafo *grafo) {
    int numCamadas = 3;
    int alturaCamada = 150;
    int espacamentoHorizontal = 100;
    int raioVertice = 20;

    int centerX = 400;
    int centerY = alturaCamada / 2;

    int numVerticesCamada = grafo->V / numCamadas;
    int espacamentoVertical = alturaCamada / (numVerticesCamada + 1);

    for (int i = 0; i < grafo->V; i++) {
        int camadaAtual = i / (grafo->V / numCamadas);

        int x = centerX + camadaAtual * espacamentoHorizontal;
        int y = centerY + (i % (numVerticesCamada) + 1) * espacamentoVertical;

        sdlRenderizarCirculo((SDL_Point){x, y}, raioVertice, (SDL_Colour){255, 0, 0, 255}, "1");
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < grafo->A; i++) {
        Aresta aresta = grafo->arestas[i];

        int camadaOrigem = aresta.v / (grafo->V / numCamadas);
        int camadaDestino = aresta.w / (grafo->V / numCamadas);

        if (camadaOrigem < camadaDestino) {
            int x1 = centerX + camadaOrigem * espacamentoHorizontal;
            int y1 = centerY + (aresta.v % (grafo->V / numCamadas) + 1) * espacamentoVertical;

            int x2 = centerX + camadaDestino * espacamentoHorizontal;
            int y2 = centerY + (aresta.w % (grafo->V / numCamadas) + 1) * espacamentoVertical;

            sdlRenderizarLinha((SDL_Point){x1, y1}, (SDL_Point){x2, y2}, (SDL_Colour){255, 0, 0, 255}, "1");
        }
    }
}

void renderizarForcaDirecionada(Grafo* grafo) {
    int larguraTela = 1280;
    int alturaTela = 720;

    // Parâmetros do algoritmo de força direcionada
    float k = 0.5; // Constante de repulsão
    float c = 0.1; // Constante de atracão
    float dMax = 200.0; // Distância máxima de atração
    float dt = 0.1; // Passo de tempo

    // Inicialização das posições dos vértices
    float* posX = (float*)malloc(grafo->V * sizeof(float));
    float* posY = (float*)malloc(grafo->V * sizeof(float));

    for (int i = 0; i < grafo->V; i++) {
        posX[i] = (float)(rand() % larguraTela);
        posY[i] = (float)(rand() % alturaTela);
    }

    // Laço principal do algoritmo de força direcionada
    for (int iter = 0; iter < 1000; iter++) {
        // Zerar as forças em cada iteração
        float* forceX = (float*)calloc(grafo->V, sizeof(float));
        float* forceY = (float*)calloc(grafo->V, sizeof(float));

        // Cálculo das forças de repulsão entre os vértices
        for (int i = 0; i < grafo->V; i++) {
            for (int j = i + 1; j < grafo->V; j++) {
                float dx = posX[i] - posX[j];
                float dy = posY[i] - posY[j];
                float dist = sqrt(dx * dx + dy * dy);
                if (dist > 0) {
                    float force = k / (dist * dist);
                    forceX[i] += force * dx;
                    forceY[i] += force * dy;
                    forceX[j] -= force * dx;
                    forceY[j] -= force * dy;
                }
            }
        }

        // Cálculo das forças de atração entre os vértices conectados
        for (int i = 0; i < grafo->A; i++) {
            Aresta aresta = grafo->arestas[i];
            float dx = posX[aresta.w] - posX[aresta.v];
            float dy = posY[aresta.w] - posY[aresta.v];
            float dist = sqrt(dx * dx + dy * dy);
            if (dist > 0 && dist < dMax) {
                float force = c * (dist - dMax) / dist;
                forceX[aresta.v] += force * dx;
                forceY[aresta.v] += force * dy;
                forceX[aresta.w] -= force * dx;
                forceY[aresta.w] -= force * dy;
            }
        }

        // Atualização das posições dos vértices
        for (int i = 0; i < grafo->V; i++) {
            posX[i] += dt * forceX[i];
            posY[i] += dt * forceY[i];
        }

        // Renderização do grafo a cada iteração
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < grafo->V; i++) {
            char label[10];
            sprintf(label, "V%d", i + 1);

            sdlRenderizarCirculo((SDL_Point){(int)posX[i], (int)posY[i]}, 10, (SDL_Colour){255, 0, 0, 255}, label);
        }

        for (int i = 0; i < grafo->A; i++) {
            Aresta aresta = grafo->arestas[i];

            char label[10];
            sprintf(label, "A%d", i + 1);

            sdlRenderizarLinha((SDL_Point){(int)posX[aresta.v], (int)posY[aresta.v]}, (SDL_Point){(int)posX[aresta.w], (int)posY[aresta.w]}, (SDL_Colour){0, 0, 255, 255}, label);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100); // Atraso para visualização
    }

    free(posX);
    free(posY);
}


void renderizarGrafo(Grafo *grafo, RenderFunction renderFunction) {
    // Configurações do layout do grafo
    // Chamar a função de renderização específica
    renderFunction(grafo);
}


