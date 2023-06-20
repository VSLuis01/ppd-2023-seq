#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>

int regraDesempate(Aresta aresta1, Aresta aresta2) {
    // Implemente sua regra de desempate personalizada aqui
    // Por exemplo, você pode comparar as arestas com base em critérios adicionais
    // e retornar 1 se a aresta1 for preferida sobre a aresta2 no caso de um empate,
    // caso contrário, retorne 0.
    // Neste exemplo, a função sempre retorna 0, o que significa que não há preferência
    // quando as arestas têm o mesmo peso.
    return 1;
}

int forPreferidoSobre(Aresta aresta1, Aresta aresta2) {
    return (aresta2.peso == -1) ||
           (aresta1.peso < aresta2.peso) ||
           (aresta1.peso == aresta2.peso && regraDesempate(aresta1, aresta2));
}

/**
 * Encontra em que componente esta o vertice v
 * @param v
 * @param componentes
 * @param quantComponentes
 * @return indice do componente que contem o vertice v ou -1 caso nao encontre
 */
int encontrarComponente(int v, Grafo **componentes, int quantComponentes) {
    for (int i = 0; i < quantComponentes; ++i) {
        if (componentes[i] != NULL) {
            for (int j = 0; j < componentes[i]->V; ++j) {
                if (componentes[i]->vertices[j].v == v) {
                    return i;
                }
            }
        }
    }
    return -1;
}

/**
 * @brief Verifica se todos os componentes estão conectados ou seja, nenhuma arvore pode ser mesclada
 * @param arestasMenorPeso
 * @param quantComponentes
 * @return true caso todos os componentes estejam conectados e false caso contrário
 */
bool todosComponentesConectados(Aresta *arestasMenorPeso, int quantComponentes) {
    bool todosComponentesConectados = true;
    for (int i = 0; i < quantComponentes; ++i) {
        if (arestasMenorPeso[i].peso != -1) {
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
 * @param quantComponentes
 */
void encontrarComponentesConectados(Grafo *arvoreGM, Grafo **componentes, int *quantComponentes) {
    // Caso nao tenha nenhuma aresta na arvore, entao cada vertice é uma componente
    if (arvoreGM->A == 0) {
        for (int i = 0; i < arvoreGM->V; ++i) {
            componentes[i] = inicializaGrafo();
            inserirVertice(componentes[i], arvoreGM->vertices[i].v);
        }
        *quantComponentes = arvoreGM->V;
    } else {
        /*Reseta as componentes*/
        for (int i = 0; i < *quantComponentes; ++i) {
            deleteGrafo(componentes[i]);
            componentes[i] = NULL;
        }
        int novaQuantComponentes = 0;
        *quantComponentes = 0;
        /*Encontrar as componentes conexas*/
        for (int i = 0; i < arvoreGM->A; ++i) {
            int componenteV = encontrarComponente(arvoreGM->arestas[i].v, componentes, *quantComponentes);
            int componenteW = encontrarComponente(arvoreGM->arestas[i].w, componentes, *quantComponentes);

            /*V e W nao pertence a nenhuma componenete. Entao crie uma componente com os dois vertices nela*/
            if (componenteV == -1 && componenteW == -1) {
                componentes[*quantComponentes] = inicializaGrafo();
                inserirVertice(componentes[*quantComponentes], arvoreGM->arestas[i].v);
                inserirVertice(componentes[*quantComponentes], arvoreGM->arestas[i].w);
                inserirAresta(componentes[*quantComponentes], arvoreGM->arestas[i].v, arvoreGM->arestas[i].w, arvoreGM->arestas[i].peso, 0);
                *quantComponentes += 1;

            } else if (componenteV != -1 && componenteW == -1) { /*Insere a componente W na mesma componente de V. Pois estao conectados*/
                inserirVertice(componentes[componenteV], arvoreGM->arestas[i].w);
                inserirAresta(componentes[componenteV], arvoreGM->arestas[i].v, arvoreGM->arestas[i].w, arvoreGM->arestas[i].peso, 0);

            } else if (componenteV == -1) { /*Insere a componente V na mesma componente de W. Pois estao conectados*/
                inserirVertice(componentes[componenteW], arvoreGM->arestas[i].v);
                inserirAresta(componentes[componenteW], arvoreGM->arestas[i].v, arvoreGM->arestas[i].w, arvoreGM->arestas[i].peso, 0);

            } else if (componenteV != componenteW) { /*Se V e W estao em componentes diferentes, entao insere a componente W na componente V*/

                for (int j = 0; j < componentes[componenteW]->V; ++j) {
                    inserirVertice(componentes[componenteV], componentes[componenteW]->vertices[j].v);
                }
                for (int j = 0; j < componentes[componenteW]->A; ++j) {
                    inserirAresta(componentes[componenteV], componentes[componenteW]->arestas[j].v, componentes[componenteW]->arestas[j].w, componentes[componenteW]->arestas[j].peso, 0);
                }
                inserirAresta(componentes[componenteV], arvoreGM->arestas[i].v, arvoreGM->arestas[i].w, arvoreGM->arestas[i].peso, 0);
                deleteGrafo(componentes[componenteW]);
                componentes[componenteW] = NULL;
                novaQuantComponentes += 1;
            }
        }
        *quantComponentes -= novaQuantComponentes;
    }
}

/**
 * @brief Constrói uma arvore geradora mínima a partir do grafo de entrada.
 * @param grafo
 * @return Arvore geradora minima F (V, E')
 */
Grafo *arvoreGeradoraMinima(Grafo *grafo) {
    Grafo *componente[grafo->V]; // 1. Inicialize uma componente F (V, E') onde E' = {}
    Grafo *arvoreGeradoraMinima = inicializaGrafoComVertice(grafo->V);

    //Inicialize uma floresta F para (V, E') onde E' = {}.
    for (int i = 0; i < grafo->V; ++i) {
        inserirVerticeDireto(arvoreGeradoraMinima, grafo->vertices[i].v);
    }

    int quantComponentes = 0;

    bool concluido = false; // 2. concluido := falso
    int iteracoes = 0;

    while (!concluido) {
        //4. Encontre os componente conectados de F
        encontrarComponentesConectados(arvoreGeradoraMinima, componente, &quantComponentes);

        // e atribua a cada vértice seu componente.
        for (int i = 0; i < quantComponentes && quantComponentes > 1; ++i) {
            for (int j = 0; j < componente[i]->V; ++j) { /*Percorrendo os vertices de cada componente*/
                // Encontrar os componente conectados em cada vertice
                for (int k = 0; k < grafo->A; ++k) { /*Percorrendo as arestas do grafo*/
                    if (grafo->arestas[k].v == componente[i]->vertices[j].v) {
                        // Se o vertice da aresta for igual ao vertice do componente, então o vertice da aresta pertence ao componente
                        inserirVertice(componente[i], grafo->arestas[k].v);
                        inserirAresta(componente[i], grafo->arestas[k].v, grafo->arestas[k].w, grafo->arestas[k].peso,0);
                    }
                    if (grafo->arestas[k].w == componente[i]->vertices[j].v) {
                        // Se o vertice da aresta for igual ao vertice do componente, então o vertice da aresta pertence ao componente
                        inserirVertice(componente[i], grafo->arestas[k].w);
                        inserirAresta(componente[i], grafo->arestas[k].v, grafo->arestas[k].w, grafo->arestas[k].peso,0);
                    }
                }
            }
        }

        Aresta arestaMenorPeso[quantComponentes]; /*Inicialize a aresta de menor peso de cada componente como nenhuma*/
        for (int k = 0; k < quantComponentes; ++k) {
            arestaMenorPeso[k].peso = -1;
        }

        // Para cada aresta uv em E, onde uv estão em diferentes componentes de F, faça:
        for (int i = 0; i < grafo->A; ++i) {
            Aresta arestaUV = grafo->arestas[i];
            int u = arestaUV.v;
            int v = arestaUV.w;

            /*Encontra o componente que possui o vertice u*/
            int componenteU = encontrarComponente(u, componente, quantComponentes);
            /*Encontra o componente que possui o vertice v*/
            int componenteV = encontrarComponente(v, componente, quantComponentes);

            if (componenteU != componenteV) { /*Aresta uv ligando diferentes componentes*/
                Aresta arestaWX = arestaMenorPeso[componenteU]; // Deixe wx ser a aresta de menor peso para a componente de u.
                Aresta arestaYZ = arestaMenorPeso[componenteV];

                if (forPreferidoSobre(arestaUV, arestaWX)) {
                    arestaMenorPeso[componenteU] = arestaUV; // Defina uv como a aresta de menor peso para a componente de u.
                    arestaMenorPeso[componenteV] = arestaYZ; // Deixe yz ser a aresta de menor peso para a componente de v.
                }
                if (forPreferidoSobre(arestaUV, arestaYZ)) {
                    arestaMenorPeso[componenteV] = arestaUV; // Defina uv como a aresta de menor peso para o componente de v.
                }
            }
        }

        /*Todos componentes tem a aresta de menor valor como Nenhuma*/
        if (todosComponentesConectados(arestaMenorPeso, quantComponentes)) {
            concluido = true;
        } else {
            concluido = false;
            for (int i = 0; i < quantComponentes; ++i) { // Para cada componente cuja aresta de menor peso não é "Nenhuma", faça:
                if (arestaMenorPeso[i].peso != -1) {
                    // Adicione sua aresta de menor peso em E'.
                    inserirAresta(arvoreGeradoraMinima, arestaMenorPeso[i].v, arestaMenorPeso[i].w, arestaMenorPeso[i].peso, 1);
                }
            }
        }
        iteracoes++;
    }


    for (int i = 0; i < quantComponentes; ++i) {
        deleteGrafo(componente[i]);
        componente[i] = NULL;
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
    Aresta *novaAresta = (Aresta *) realloc(grafo->arestas,
                                            grafo->A * sizeof(Aresta)); // Realocar memória para a nova aresta
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
    if (grafo->vertices[v - 1].v == -1) {
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
    grafo->vertices = (Vertice *) malloc(numVertices * sizeof(Vertice));
    for (int i = 0; i < numVertices; ++i) {
        grafo->vertices[i].v = -1;
        grafo->vertices[i].grau = 0;
    }
    return grafo;
}
