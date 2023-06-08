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
 * @brief Insere Vertices no conjunto de vertices
 * @param grafo ponteiro para o grafo
 * @param v vertice
 * @param w vertice
 */
void inserirVertice(Grafo* grafo, int v, int w) {
    // Verifica se o vértice v já está presente no vetor de vértices
    for (int i = 0; i < grafo->V; i++) {
        if (grafo->vertices[i].v == v) {
            grafo->vertices[i].grau++;
            return;  // O vértice já existe, incrementa o grau e retorna
        }
    }

    // Verifica se o vértice w já está presente no vetor de vértices
    for (int i = 0; i < grafo->V; i++) {
        if (grafo->vertices[i].v == w) {
            grafo->vertices[i].grau++;
            return;  // O vértice já existe, incrementa o grau e retorna
        }
    }

    // Cria um novo vértice e insere no vetor de vértices
    Vertice novoVerticeV = {v, 1};
    grafo->vertices = realloc(grafo->vertices, (grafo->V + 1) * sizeof(Vertice));
    grafo->vertices[grafo->V++] = novoVerticeV;

    // Se o vértice v e w são diferentes, insere também o vértice w
    if (v != w) {
        Vertice novoVerticeW = {w, 1};
        grafo->vertices = realloc(grafo->vertices, (grafo->V + 1) * sizeof(Vertice));
        grafo->vertices[grafo->V++] = novoVerticeW;
    } else {
        // Incrementa o grau em 2 caso v e w sejam iguais, que é loop
        grafo->vertices[grafo->V - 1].grau += 2;
    }
}


/**
 * @brief Libera a memória alocada para o grafo e para o conjunto de arestas
 * @param grafo Ponteiro para o grafo
 */
void deleteGrafo(Grafo* grafo) {
    free(grafo->arestas);
    free(grafo->vertices);
    free(grafo);
}

/**
 * @brief Inicializa o grafo. Aloca memória para o grafo e para o conjunto de arestas e inicializa o número de vértices e arestas
 * @return Ponteiro para o grafo
 */
Grafo* inicializaGrafo() {
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->V = 0;
    grafo->A = 0;  // Iniciar o número de arestas como 0
    grafo->arestas = NULL; // Inicialmente, não há arestas alocadas
    grafo->vertices = NULL;
    return grafo;
}
