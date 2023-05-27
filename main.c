#include <stdio.h>
#include "structures/grafo.h"
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
    }
}

int main() {
    char nomeDoArquivo[] = "dados_entrada_sequencial.txt";
    int numVertices = NUM_VERTICES;
    int running = 1;

    sdlInitWindow(800, 600);

    while (running) {
        while (sdlPullEvent()) {
            if (sdlQuit()) {
                // Encerra o programa se o usuário fechar a janela
                sdlDestroyWindow();
                running = 0;
            }

            sdlRender();
        }
    }

    /*
    Grafo *grafo = inicializaGrafo(numVertices);

    construirGrafo(grafo, nomeDoArquivo);

    printGrafo(grafo);

    deleteGrafo(grafo);
     */
    return 0;
}
