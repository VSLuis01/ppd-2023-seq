//
// Created by luis on 26/05/23.
//

#ifndef PPD_2023_SEQ_GRAFO_H
#define PPD_2023_SEQ_GRAFO_H
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
 * @brief Inicializa o grafo. Aloca memória para o grafo e para o conjunto de arestas e inicializa o número de vértices e arestas
 * @param V Número de vértices
 * @param A Número de arestas
 * @return Ponteiro para o grafo
 */
Grafo* inicializaGrafo(int V);
/**
 * @brief Insere aresta no grafo e incrementa o número de arestas
 * @param grafo Ponteiro para o grafo
 * @param v vertice 1
 * @param w vertice 2
 * @param peso
 */
int inserirAresta(Grafo* grafo, int v, int w, int peso);

/**
 * @brief Libera a memória alocada para o grafo e para o conjunto de arestas
 * @param grafo Ponteiro para o grafo
 */
void deleteGrafo(Grafo* grafo);


#endif //PPD_2023_SEQ_GRAFO_H
