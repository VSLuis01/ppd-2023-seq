#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int regraDesempate() {
    return 1;
}

int forPreferidoSobre(Aresta aresta1, Aresta aresta2) {
    return (aresta2.peso == -1) || (aresta1.peso < aresta2.peso) || (aresta1.peso == aresta2.peso && regraDesempate());
}

/**
 * @brief Verifica se todos os componentes estão conectados ou seja, nenhuma arvore pode ser mesclada
 * @param arestasMenorPeso
 * @param quantComponentes
 * @return true caso todos os componentes estejam conectados e false caso contrário
 */
bool todosComponentesConectados(Aresta *arestasMenorPeso, const int *componentes, int quantComponentes) {
    bool todosComponentesConectados = true;
    for (int i = 0; i < quantComponentes; ++i) {
        if (arestasMenorPeso[componentes[i]].peso != -1) {
            todosComponentesConectados = false;
            break;
        }
    }
    return todosComponentesConectados;
}

/**
 * Encontra os componentes conectados em uma arvore geradora minima
 * @param arvoreGM
 * @param componentes
 * @param componentes
 */
void encontrarComponentesConectados(Grafo *arvoreGM, int *componentes, int quantComponentes) {
    // Caso nao tenha nenhuma aresta na arvore, entao cada vertice é uma componente
    if (arvoreGM->A == 0) {
        // Atribuir a cada vertice seu componente
        for (int i = 0; i < arvoreGM->V; ++i) {
            arvoreGM->vertices[i].componente = arvoreGM->vertices[i].v - 1;
            componentes[i] = arvoreGM->vertices[i].v - 1;
        }

    } else {
        int j = 0;

        memset(componentes, -1, quantComponentes * sizeof(int));

        for (int i = 0; i < arvoreGM->A; ++i) {
            int v = arvoreGM->arestas[i].v;
            int w = arvoreGM->arestas[i].w;

            int componenteV = arvoreGM->vertices[v - 1].componente;
            int componenteW = arvoreGM->vertices[w - 1].componente;

            if (componenteV != componenteW) {
                if (componenteV < componenteW) {
                    arvoreGM->vertices[w - 1].componente = componenteV;

                    // Verifica se a componente já nao foi inserida.
                    bool componenteVRepetida = false;
                    for (int k = 0; k < j && j < quantComponentes; ++k) {
                        if (componentes[k] == componenteV) {
                            componenteVRepetida = true;
                            break;
                        }
                    }

                    if (!componenteVRepetida) {
                        componentes[j] = componenteV;
                        j++;
                    }
                } else {

                    // Verifica se a componente já nao foi inserida.
                    arvoreGM->vertices[v - 1].componente = componenteW;

                    bool componenteWRepetida = false;
                    for (int k = 0; k < j && j < quantComponentes; ++k) {
                        if (componentes[k] == componenteW) {
                            componenteWRepetida = true;
                            break;
                        }
                    }

                    if (!componenteWRepetida) {
                        componentes[j] = componenteW;
                        j++;
                    }
                }
            }
        }
    }
}

/**
 * @brief Constrói uma arvore geradora mínima a partir do grafo de entrada.
 * @param grafo
 * @return Arvore geradora minima F (V, E')
 */
Grafo *arvoreGeradoraMinima(Grafo *grafo) {
    // 1. Inicialize uma floresta F para (V, E') onde E' = {}.
    Grafo *arvoreGeradoraMinima = inicializaGrafoComVertice(grafo->V);
    memcpy(arvoreGeradoraMinima->vertices, grafo->vertices, grafo->V * sizeof(Vertice));

    bool concluido = false; // 2. concluido := falso
    int iteracoes = 0;

    int quantComponentes = arvoreGeradoraMinima->V - arvoreGeradoraMinima->A;
    Aresta arestaMenorPeso[quantComponentes];
    int *componentes;

    while (!concluido) {

        quantComponentes = arvoreGeradoraMinima->V - arvoreGeradoraMinima->A;

        componentes = (int *) malloc(quantComponentes * sizeof(int));
        // 4. Encontre os componentes conectados de F e atribua a cada vértice seu componente.
        encontrarComponentesConectados(arvoreGeradoraMinima, componentes, quantComponentes);

        // 5. Inicialize a aresta de menor peso para cada componente como "Nenhuma".
        for (int k = 0; k < quantComponentes; ++k) {
            int componente = componentes[k];
            arestaMenorPeso[componente].peso = -1;
        }

        // 6. Para cada aresta uv em E, onde uv estão em diferentes componentes de F, faça:
        for (int i = 0; i < grafo->A; ++i) {
            Aresta arestaUV = grafo->arestas[i];
            int u = arestaUV.v;
            int v = arestaUV.w;

            /*Encontra o componente que possui o vertice u*/
            int componenteU = arvoreGeradoraMinima->vertices[u - 1].componente;
            /*Encontra o componente que possui o vertice v*/
            int componenteV = arvoreGeradoraMinima->vertices[v - 1].componente;

            if (componenteU != componenteV) { /*Aresta uv ligando diferentes componentes*/
                // 7. Deixe wx ser a aresta de menor peso para a componente de u.
                Aresta arestaWX = arestaMenorPeso[componenteU];
                Aresta arestaYZ = arestaMenorPeso[componenteV];

                // 8. Se for-preferido(uv, wx) então:
                if (forPreferidoSobre(arestaUV, arestaWX)) {
                    // 9. Defina uv como a aresta de menor peso para a componente de u.
                    arestaMenorPeso[componenteU] = arestaUV;
                    // 10. Deixe yz ser a aresta de menor peso para a componente de v.
                    arestaMenorPeso[componenteV] = arestaYZ;
                }
                // 11. Se for-preferido-sobre(uv, yz) então:
                if (forPreferidoSobre(arestaUV, arestaYZ)) {
                    // 12. Defina uv como a aresta de menor peso para o componente de v.
                    arestaMenorPeso[componenteV] = arestaUV;
                }
            }
        }

        // 13. Se todos os componentes tiverem a aresta de menor peso definida como "Nenhum" então:
        if (todosComponentesConectados(arestaMenorPeso, componentes, quantComponentes)) {
            // 15. completo := verdadeiro.
            concluido = true; // Nenhuma outra árvore pode ser mesclada - nós terminamos.
        } else {
            // 17. completo := falso.
            concluido = false;
            // Para cada componente cuja aresta de menor peso não é "Nenhuma", faça:
            for (int i = 0; i < quantComponentes; ++i) {
                int indexComponente = componentes[i];
                if (arestaMenorPeso[indexComponente].peso != -1) {
                    // Adicione sua aresta de menor peso em E'.
                    inserirAresta(arvoreGeradoraMinima,
                                  arestaMenorPeso[indexComponente].v,
                                  arestaMenorPeso[indexComponente].w,
                                  arestaMenorPeso[indexComponente].peso,
                                  1);
                }
            }
        }
        iteracoes++;
        free(componentes);
    }
    return arvoreGeradoraMinima;
}


/**
 * Insere a aresta de forma dinamica.
 * @param grafo
 * @param v
 * @param w
 * @param peso
 * @param somarGrau flag para somar o grau do vertice
 * @return 1 caso inserido com sucesso, 0 caso contrario
 */
int inserirAresta(Grafo *grafo, int v, int w, int peso, int somarGrau) {
    for (int i = 0; i < grafo->A; ++i) {
        if (grafo->arestas[i].v == v && grafo->arestas[i].w == w && grafo->arestas[i].peso == peso) {
            // Aresta já existe
            return 0;
        }
    }
    grafo->A++; // Incrementar o número de arestas
    Aresta *novaAresta = (Aresta *) realloc(grafo->arestas, grafo->A * sizeof(Aresta)); // Realocar memória para a nova aresta
    if (novaAresta == NULL) {
        // Tratar a falha de alocação de memória
        return 0;
    }
    grafo->arestas = novaAresta; // Atribuir a nova memória realocada
    int posicao = grafo->A - 1; // Posição correta no array de arestas
    grafo->arestas[posicao].v = v;
    grafo->arestas[posicao].w = w;
    grafo->arestas[posicao].peso = peso;
    if (somarGrau) {
        grafo->vertices[v - 1].grau++;
        grafo->vertices[w - 1].grau++;
    }
    return 1;
}

/**
 * @brief Insere o vertice diretamente no seu indice,
 * @attention Essa funcao so funciona quando o grafo eh iniciado com a funcao Grafo* inicializaGrafoComVertice(int numVertices);
 * @attention Nessa funcao, os indices do conjunto de vertices sao iguais ao (valor do vertice - 1).
 * @param grafo
 * @param v
 */
void inserirVerticeDireto(Grafo *grafo, int v) {
    if (grafo->vertices[v - 1].v == 0) {
        grafo->vertices[v - 1].v = v;
    }
}

/**
 * @brief Insere vertices de forma dinamica.
 * @attention Essa funcao eh melhor utilizada quando o grafo eh iniciado com a funcao Grafo *inicializaGrafo();
 * @attention Nessa funcao, os indices do conjunto de vertices NAO sao iguais ao valor do vertice.
 * @param grafo ponteiro para o grafo
 * @param v vertice
 * @param w vertice
 */
void inserirVertice(Grafo *grafo, int v) {
    // Verifica se o vértice v já está presente no vetor de vértices
    for (int i = 0; i < grafo->V; i++) {
        if (grafo->vertices[i].v == v) {
            return;  // O vértice já existe, incrementa o grau e retorna
        }
    }

    // Cria um novo vértice e insere no vetor de vértices
    Vertice novoVerticeV = {v, 0};
    grafo->vertices = realloc(grafo->vertices, (grafo->V + 1) * sizeof(Vertice));
    grafo->vertices[grafo->V++] = novoVerticeV;

}

/**
 * @brief Libera a memória alocada para o grafo e para o conjunto de arestas
 * @param grafo Ponteiro para o grafo
 */
void deleteGrafo(Grafo *grafo) {
    free(grafo->arestas);
    grafo->arestas = NULL;
    free(grafo->vertices);
    grafo->vertices = NULL;
    free(grafo);
}

/**
 * @brief Inicializa o grafo. Aloca memória para o grafo e para o conjunto de arestas e inicializa o número de vértices e arestas
 * @return Ponteiro para o grafo
 */
Grafo *inicializaGrafo() {
    Grafo *grafo = (Grafo *) malloc(sizeof(Grafo));
    grafo->V = 0;
    grafo->A = 0;  // Iniciar o número de arestas como 0
    grafo->arestas = NULL; // Inicialmente, não há arestas alocadas
    grafo->vertices = NULL;
    return grafo;
}

Grafo *inicializaGrafoComVertice(int numVertices) {
    Grafo *grafo = (Grafo *) malloc(sizeof(Grafo));
    grafo->V = numVertices;
    grafo->A = 0;  // Iniciar o número de arestas como 0
    grafo->arestas = NULL; // Inicialmente, não há arestas alocadas
    grafo->vertices = (Vertice *) calloc(numVertices, sizeof(Vertice));

    return grafo;
}
