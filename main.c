#include "graphics/sdl_utils.h"
#include <stdio.h>

#define NUM_VERTICES 9


void printGrafo(Grafo *grafo) {
    int totalPeso = 0;
    printf("Número de vértices: %d\n", grafo->V);
    printf("Número de arestas: %d\n", grafo->A);
    printf("Arestas: \n");
    for (int i = 0; i < grafo->A; i++) {
        printf("Aresta %d: %d - %d (peso: %d)\n", i, grafo->arestas[i].v, grafo->arestas[i].w, grafo->arestas[i].peso);
        totalPeso += grafo->arestas[i].peso;
    }
    printf("Peso total: %d\n", totalPeso);
    for (int i = 0; i < grafo->V; ++i) {
        printf("%d - GRAU (%d)\n", grafo->vertices[i].v, grafo->vertices[i].grau);
    }
}

/**
 * @brief Procedimento auxiliar para construir o grafo a partir de um arquivo de entrada
 * @param grafo Ponteiro para o ponteiro do grafo
 * @param nomeDoArquivo nome do arquivo de entrada
 */
void construirGrafo(Grafo *grafo, char *nomeDoArquivo) {
    FILE *file;

    file = fopen(nomeDoArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeDoArquivo);
        return;
    }

    while (!feof(file)) {
        int v, w, peso;
        fscanf(file, "%*d %d %d %d", &v, &w, &peso);
        inserirVerticeDireto(grafo, v);
        inserirVerticeDireto(grafo, w);
        inserirAresta(grafo, v, w, peso, 1);
    }
}

int main() {
    printf("Comandos:\n - Espaço: Mostrar AGM\n - Seta para cima: Aumentar seed\n - Seta para baixo: Diminuir seed\n - R: Resetar seed\n");

    char nomeDoArquivo[] = "dados_entrada_sequencial.txt";
    int running = 1;

    Grafo *grafo = inicializaGrafoComVertice(NUM_VERTICES);

    construirGrafo(grafo, nomeDoArquivo);

    Grafo *agm = arvoreGeradoraMinima(grafo);

    sdlInitWindow(1600, 1300);

    int renderizarAGM = 0;
    while (running) {
        sdlClearRender();

        if (renderizarAGM) {
            renderizarGrafo(agm, desenharGrafo);
        } else {
            renderizarGrafo(grafo, desenharGrafo);
        }

        sdlDraw();
        while (sdlPullEvent()) {
            if (sdlEventClosedWindow()) {
                sdlDestroyWindow();
                running = 0;
                break;
            } else if (sdlEventKeyDown()) {
                if (sdlGetKeyCode() == SDLK_SPACE) {
                    renderizarAGM = !renderizarAGM;
                } else if (sdlGetKeyCode() == SDLK_UP) {
                    changeSeed(1);
                } else if (sdlGetKeyCode() == SDLK_DOWN) {
                    changeSeed(-1);
                } else if (sdlGetKeyCode() == SDLK_r) {
                    setSeed(20);
                } else if (sdlGetKeyCode() == SDLK_ESCAPE) {
                    sdlDestroyWindow();
                    running = 0;
                    break;
                }
            }
        }
    }


    deleteGrafo(grafo);
    grafo = NULL;
    deleteGrafo(agm);
    agm = NULL;

    return 0;
}
