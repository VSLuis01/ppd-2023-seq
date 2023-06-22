#include "graphics/sdl_utils.h"
#include <stdio.h>

#define NUM_VERTICES 91


void printGrafo(Grafo *grafo) {
    int totalPeso = 0;
    printf("Número de vértices: %d\n", grafo->V);
    printf("Número de arestas: %d\n", grafo->A);
    printf("Arestas: \n");
    for (int i = 0; i < grafo->A; i++) {
        printf("Aresta %d: %d - %d (peso: %d)\n", i, grafo->arestas[i].v, grafo->arestas[i].w, grafo->arestas[i].peso);
        totalPeso += grafo->arestas[i].peso;
    }
    printf("Peso total: %d\n", totalPeso);
   /* for (int i = 0; i < grafo->V; ++i) {
        printf("%d - GRAU (%d)\n", grafo->vertices[i].v, grafo->vertices[i].grau);
    }*/
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
        inserirVerticeDireto(grafo, v);
        inserirVerticeDireto(grafo, w);
        inserirAresta(grafo, v, w, peso, 1);
    }
}

int main(int argc, char** argv) {
    printf("Comandos:\n - Espaço: Mostrar AGM\n - Seta para cima: Aumentar seed\n - Seta para baixo: Diminuir seed\n - R: Resetar seed\n");

    char nomeDoArquivo[] = "dados_entrada_sequencial.txt";
    int running = 1;

    int numVertices;
    if (argc == 2) {
        int numVerticesArg = atoi(argv[1]);
        numVertices = numVerticesArg > 0 ? numVerticesArg : NUM_VERTICES;
    } else {
        numVertices = NUM_VERTICES;
    }

    Grafo *grafo = inicializaGrafoComVertice(numVertices);


    construirGrafo(grafo, nomeDoArquivo);

    Grafo *agm = arvoreGeradoraMinima(*grafo);
    printf("Grafo Original:\n");
    printGrafo(grafo);
    printf("Grafo AGM:\n");
    printGrafo(agm);

    sdlInitWindow(1300, 800);

    int renderizarAGM = 0;
    while (running) {
        sdlClearRender();

        if (renderizarAGM) {
            renderizarGrafo(agm, desenharGrafo);
        } else {
            renderizarGrafo(grafo, desenharGrafo);
        }

        sdlDraw();
        while (sdlPullEvent()) {
            if (sdlEventClosedWindow()) {
                sdlDestroyWindow();
                running = 0;
                break;
            } else if (sdlEventKeyDown()) {
                if (sdlGetKeyCode() == SDLK_SPACE) {
                    renderizarAGM = !renderizarAGM;
                } else if (sdlGetKeyCode() == SDLK_UP) {
                    changeSeed(1);
                } else if (sdlGetKeyCode() == SDLK_DOWN) {
                    changeSeed(-1);
                } else if (sdlGetKeyCode() == SDLK_r) {
                    setSeed(16);
                } else if (sdlGetKeyCode() == SDLK_ESCAPE) {
                    sdlDestroyWindow();
                    running = 0;
                    break;
                }
            }
        }
    }

    deleteGrafo(agm);
    agm = NULL;
    deleteGrafo(grafo);
    grafo = NULL;

    return 0;
}
