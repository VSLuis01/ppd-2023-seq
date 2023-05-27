#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v; /*Vértice 1*/
    int w; /*Vértice 2*/
    int peso;
} Aresta;

typedef struct {
    int V; /*Número de vertices no grafo*/
    int A; /*Número de arestas no grafo*/
    Aresta *arestas; /*Conjunto de arestas*/
} Grafo;

/**
 * @brief Insere aresta no grafo e incrementa o número de arestas
 * @param grafo Ponteiro para o grafo
 * @param v vertice 1
 * @param w vertice 2
 * @param peso
 */
void inserirAresta(Grafo* grafo, int v, int w, int peso) {
    grafo->A++; // Incrementar o número de arestas
    Aresta* novaAresta = (Aresta*) realloc(grafo->arestas, grafo->A * sizeof(Aresta)); // Realocar memória para a nova aresta
    if (novaAresta == NULL) {
        // Tratar a falha de alocação de memória
        printf("Erro: Falha na alocação de memória.\n");
        return;
    }
    grafo->arestas = novaAresta; // Atribuir a nova memória realocada
    int posicao = grafo->A - 1; // Posição correta no array de arestas
    grafo->arestas[posicao].v = v;
    grafo->arestas[posicao].w = w;
    grafo->arestas[posicao].peso = peso;
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
Grafo* inicializaGrafo(int V, int A) {
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->V = V;
    grafo->A = 0;  // Iniciar o número de arestas como 0
    grafo->arestas = NULL; // Inicialmente, não há arestas alocadas
    return grafo;
}

/**
 * @brief Procedimento auxiliar para exibir o grafo. Exibe o número de vértices, o número de arestas e as arestas
 * @param grafo
 */
void printGrafo(Grafo* grafo) {
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

    Grafo* grafo = inicializaGrafo(numVertices, numArestas);

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
