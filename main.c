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
    char nomeDoArquivo[] = "dados_entrada_sequencial.txt";
    int running = 1;

    Grafo *grafo = inicializaGrafoComVertice(NUM_VERTICES);

    construirGrafo(grafo, nomeDoArquivo);

    Grafo *agm = arvoreGeradoraMinima(grafo);

//    printGrafo(agm);
    /*sdlInitWindow(1600, 1300);

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
    }*/



//    printGrafo(grafo);

    deleteGrafo(grafo);
    grafo = NULL;
    deleteGrafo(agm);
    agm = NULL;

    return 0;
}
