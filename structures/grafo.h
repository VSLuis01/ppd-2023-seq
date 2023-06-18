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
    int v;
    int grau;
} Vertice;

typedef struct {
    int V; /*Número de vertices no grafo*/
    int A; /*Número de arestas no grafo*/
    Vertice *vertices; /*Conjunto de vertices*/
    Aresta *arestas; /*Conjunto de arestas*/
} Grafo;

void printGrafo(Grafo *grafo);

/**
 * @brief Constrói uma arvore geradora mínima a partir do grafo de entrada.
 * @param grafo
 * @return Arvore geradora minima F (V, E')
 */
Grafo* arvoreGeradoraMinima(Grafo *grafo);

/**
 * @brief Inicializa o grafo. Aloca memória para o grafo e para o conjunto de arestas e inicializa o número de vértices e arestas
 * @return Ponteiro para o grafo
 */
Grafo* inicializaGrafo();

Grafo* inicializaGrafoComVertice(int numVertices);

Grafo* inicializaGrafoGerador(Grafo grafo);
/**
 * @brief Insere aresta no grafo e incrementa o número de arestas
 * @param grafo Ponteiro para o grafo
 * @param v vertice 1
 * @param w vertice 2
 * @param peso
 */
int inserirAresta(Grafo* grafo, int v, int w, int peso, int somarGrau);

/**
 * @brief Insere Vertices no conjunto de vertices
 * @param grafo ponteiro para o grafo
 * @param v vertice
 * @param w vertice
 */
void inserirVertice(Grafo* grafo, int v);

void inserirVerticeDireto(Grafo* grafo, int v);

/**
 * @brief Libera a memória alocada para o grafo e para o conjunto de arestas
 * @param grafo Ponteiro para o grafo
 */
void deleteGrafo(Grafo* grafo);


#endif //PPD_2023_SEQ_GRAFO_H
