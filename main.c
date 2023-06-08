#include <stdio.h>
#include "graphics/sdl_utils.h"

#define NUM_VERTICES 91

/**
 * @brief Procedimento auxiliar para exibir o grafo. Exibe o número de vértices, o número de arestas e as arestas
 * @param grafo
 */
void printGrafo(Grafo *grafo) {
    int totalPeso = 0;
    printf("Número de vértices: %d\n", grafo->V);
    printf("Número de arestas: %d\n", grafo->A);
    printf("Arestas:\n");
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
        inserirAresta(grafo, v, w, peso);
        inserirVertice(grafo, v, w);
    }
}

int main() {
    char nomeDoArquivo[] = "dados_entrada_sequencial.txt";
    int running = 1;

    Grafo *grafo = inicializaGrafo();

    construirGrafo(grafo, nomeDoArquivo);

    sdlInitWindow(1600, 1300);

    while (running) {
        sdlClearRender();

        renderizarGrafo(grafo, desenharGrafo);

        sdlDraw();
        while (sdlPullEvent()) {
            if (sdlEventClosedWindow()) {
                sdlDestroyWindow();
                running = 0;
                break;
            }
        }
    }



//    printGrafo(grafo);

    deleteGrafo(grafo);

    return 0;
}
