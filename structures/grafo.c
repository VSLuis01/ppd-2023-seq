#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>


int regraDesempate(Aresta aresta1, Aresta aresta2) {
    // Implemente a sua regra de desempate aqui
    return 1; // Por padrão, a primeira aresta é preferida sobre a segunda
}

int forPreferidoSobre(Aresta aresta1, Aresta aresta2) {
    return (aresta2.v == -1) || (aresta1.peso < aresta2.peso) || (aresta1.peso == aresta2.peso && regraDesempate(aresta1, aresta2));
}

/**
 * @brief Constrói uma arvore geradora mínima a partir do grafo de entrada.
 * @param grafo
 * @return Arvore geradora minima F (V, E')
 */
Grafo* arvoreGeradoraMinima(Grafo* grafo) {
    Grafo* agm = inicializaGrafoGerador(*grafo);

    // Inicializar a floresta AGM
    for (int i = 0; i < grafo->V; i++) {
        agm->vertices[i].v = grafo->vertices[i].v;
        agm->vertices[i].grau = 0;
    }

    bool concluido = false;
    while (!concluido) {
        // Encontrar os componentes conectados de AGM e atribuir a cada vértice seu componente
        int* componente = (int*)malloc(grafo->V * sizeof(int));
        for (int i = 0; i < grafo->V; i++) {
            componente[grafo->vertices[i].v - 1] = grafo->vertices[i].v;
        }

        Aresta* menorPeso = (Aresta*)malloc(grafo->V * sizeof(Aresta));
        for (int i = 0; i < grafo->V; i++) {
            menorPeso[i].v = -1;
            menorPeso[i].w = -1;
            menorPeso[i].peso = INT_MAX; // Adicione essa linha para inicializar o peso com um valor máximo
        }

        bool todasArestasNenhuma = true;
        for (int i = 0; i < grafo->A; i++) {
            Aresta aresta = grafo->arestas[i];
            int componenteU = componente[aresta.v - 1];
            int componenteV = componente[aresta.w - 1];

            if (componenteU != componenteV) {
                todasArestasNenhuma = false;

                if (forPreferidoSobre(aresta, menorPeso[componenteU])) {
                    menorPeso[componenteU] = aresta;
                }

                if (forPreferidoSobre(aresta, menorPeso[componenteV])) {
                    menorPeso[componenteV] = aresta;
                }
            }
        }

        if (todasArestasNenhuma) {
            concluido = true;
        } else {
            concluido = false;
            for (int i = 0; i < grafo->V; i++) {
                if (menorPeso[i].v != -1) {
                    agm->arestas = (Aresta*)realloc(agm->arestas, (agm->A + 1) * sizeof(Aresta));
                    agm->arestas[agm->A] = menorPeso[i];
                    agm->vertices[menorPeso[i].v].grau++;
                    agm->vertices[menorPeso[i].w].grau++;
                    agm->A++;
                    componente[menorPeso[i].w] = componente[menorPeso[i].v];
                }
            }
        }

        free(componente);
        free(menorPeso);
    }

    return agm;
}

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

/**
 * @brief Inicializa o grafoGerador. Aloca memória para o grafo e para o conjunto de verticees e inicializa o número de vértices
 * @return Ponteiro para o grafo
 */
Grafo* inicializaGrafoGerador(Grafo grafo) {
    Grafo* grafoGerador = (Grafo*) malloc(sizeof(Grafo));
    grafoGerador->V = grafo.V;
    grafoGerador->A = 0;  // Iniciar o número de arestas como 0
    grafoGerador->arestas = NULL; // Inicialmente, não há arestas alocadas
    grafoGerador->vertices = (Vertice*) malloc(grafo.V * sizeof(Vertice));
    return grafoGerador;
}

