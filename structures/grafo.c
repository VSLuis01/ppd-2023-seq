#include "grafo.h"
#include <stdlib.h>

/**
 * @brief Insere aresta no grafo e incrementa o número de arestas
 * @param grafo Ponteiro para o grafo
 * @param v vertice 1
 * @param w vertice 2
 * @param peso
 */
int inserirAresta(Grafo* grafo, int v, int w, int peso) {
    grafo->A++; // Incrementar o número de arestas
    Aresta* novaAresta = (Aresta*) realloc(grafo->arestas, grafo->A * sizeof(Aresta)); // Realocar memória para a nova aresta
    if (novaAresta == NULL) {
        // Tratar a falha de alocação de memória
        return -1;
    }
    grafo->arestas = novaAresta; // Atribuir a nova memória realocada
    int posicao = grafo->A - 1; // Posição correta no array de arestas
    grafo->arestas[posicao].v = v;
    grafo->arestas[posicao].w = w;
    grafo->arestas[posicao].peso = peso;
    return 0;
}

/**
 * @brief Libera a memória alocada para o grafo e para o conjunto de arestas
 * @param grafo Ponteiro para o grafo
 */
void deleteGrafo(Grafo* grafo) {
    free(grafo->arestas);
    free(grafo);
}

/**
 * @brief Inicializa o grafo. Aloca memória para o grafo e para o conjunto de arestas e inicializa o número de vértices e arestas
 * @param V Número de vértices
 * @param A Número de arestas
 * @return Ponteiro para o grafo
 */
Grafo* inicializaGrafo(int V) {
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->V = V;
    grafo->A = 0;  // Iniciar o número de arestas como 0
    grafo->arestas = NULL; // Inicialmente, não há arestas alocadas
    return grafo;
}
