#include <stdio.h>
#include "structures/grafo.h"


/**
 * @brief Procedimento auxiliar para exibir o grafo. Exibe o número de vértices, o número de arestas e as arestas
 * @param grafo
 */
void printGrafo(Grafo *grafo) {
    printf("Número de vértices: %d\n", grafo->V);
    printf("Número de arestas: %d\n", grafo->A);
    printf("Arestas:\n");
    for (int i = 0; i < grafo->A; i++) {
        printf("Aresta %d: %d - %d (peso: %d)\n", i, grafo->arestas[i].v, grafo->arestas[i].w, grafo->arestas[i].peso);
    }
}

int main() {
    int numVertices = 5;
    int numArestas = 7;

    Grafo *grafo = inicializaGrafo(numVertices, numArestas);

    inserirAresta(grafo, 0, 1, 2);
    inserirAresta(grafo, 0, 2, 5);
    inserirAresta(grafo, 1, 2, 1);
    inserirAresta(grafo, 1, 3, 3);
    inserirAresta(grafo, 2, 3, 2);
    inserirAresta(grafo, 2, 4, 4);
    inserirAresta(grafo, 3, 4, 1);

    printGrafo(grafo);

    deleteGrafo(grafo);
    return 0;
}
