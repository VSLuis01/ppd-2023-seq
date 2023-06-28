#include "graphics/sdl_utils.h"
#include "structures/grafo.h"
#include <stdio.h>

#define NUM_VERTICES 91


void printGrafo(Grafo grafo, int exibirVertices) {
    int totalPeso = 0;
    printf("Número de vértices: %d\n", grafo.V);
    printf("Número de arestas: %d\n", grafo.A);
    printf("Arestas: \n");
    for (int i = 0; i < grafo.A; i++) {
        printf("Aresta %d: %d - %d (peso: %d)\n", i, grafo.arestas[i].v, grafo.arestas[i].w, grafo.arestas[i].peso);
        totalPeso += grafo.arestas[i].peso;
    }
    printf("Peso total: %d\n", totalPeso);
    if (exibirVertices == 1) {
        for (int i = 0; i < grafo.V; ++i) {
            printf("%d - GRAU (%d)\n", grafo.vertices[i].v, grafo.vertices[i].grau);
        }
    }
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
    int printGrafos = 0;
    int exibirVertices = 0;

    if (argc >= 2) {
        int argIndex = 1;

        if (argv[argIndex][0] == 'v') {
            printGrafos = 1;
            numVertices = NUM_VERTICES;

            if (argv[argIndex][1] == 'v') {
                exibirVertices = 1;
            }

            argIndex++;
        } else {
            int numVerticesArg = atoi(argv[argIndex]);
            numVertices = numVerticesArg > 0 ? numVerticesArg : NUM_VERTICES;
            argIndex++;
        }

        if (argIndex < argc && argv[argIndex][0] == 'v') {
            printGrafos = 1;

            if (argv[argIndex][1] == 'v') {
                exibirVertices = 1;
            }
        }
    } else {
        numVertices = NUM_VERTICES;
    }

    Grafo *grafo = inicializaGrafoComVertice(numVertices);


    construirGrafo(grafo, nomeDoArquivo);

    Grafo *agm = arvoreGeradoraMinima(*grafo);

    if (printGrafos) {
        printf("Grafo Original:\n");
        printGrafo(*grafo, exibirVertices);
        printf("Grafo AGM:\n");
        printGrafo(*agm, exibirVertices);
    }

    sdlInitWindow(1300, 800);

    int renderizarAGM = 0;
    while (running) {
        sdlClearRender();


        if (!renderizarAGM) {
            desenharGrafo(grafo->arestas, grafo->A, grafo->V, (SDL_Colour) {200, 200, 200, 255});
        } else {
            desenharGrafo(grafo->arestas, grafo->A, grafo->V, (SDL_Colour) {100, 100, 100, 255});
        }

        if (renderizarAGM) {
            desenharGrafo(agm->arestas, agm->A, agm->V, (SDL_Colour) {0, 255, 0, 255});
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
