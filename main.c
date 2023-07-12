#include "graphics/sdl_utils.h"
#include "structures/grafo.h"
#include <stdio.h>

void printGrafo(Grafo grafo) {
    u_int64_t totalPeso = 0;
    printf("Número de vértices: %lu\n", grafo.V);
    printf("Número de arestas: %lu\n", grafo.A);
    printf("Arestas: \n");
    for (u_int64_t i = 0; i < grafo.A; i++) {
//        printf("Aresta %d: %d - %d (peso: %d)\n", i, grafo.arestas[i].v, grafo.arestas[i].w, grafo.arestas[i].peso);
        totalPeso += grafo.arestas[i].peso;
    }
    printf("Peso total: %lu\n", totalPeso);
}

void lerQuantidadeVerticeArestas(u_int64_t *vertices, u_int64_t *arestas, FILE *file) {
    fscanf(file, "%lu", vertices);
    fscanf(file, "%lu", arestas);
}

/**
 * @brief Procedimento auxiliar para construir o grafo a partir de um arquivo de entrada
 * @param grafo Ponteiro para o ponteiro do grafo
 * @param nomeDoArquivo nome do arquivo de entrada
 */
void construirGrafo(Grafo *grafo, FILE *file) {
    while (!feof(file)) {
        u_int64_t v, w, peso;
        fscanf(file, "%lu %lu %lu", &v, &w, &peso);
        inserirVerticeDireto(grafo, v);
        inserirVerticeDireto(grafo, w);
        inserirAresta(grafo, v, w, peso, 1);
    }
}

int main(int argc, char **argv) {
    printf("Comandos:\n - Espaço: Mostrar AGM\n - Seta para cima: Aumentar seed\n - Seta para baixo: Diminuir seed\n - R: Resetar seed\n");

    FILE *file;
    int running = 1;

    if (argc != 2) {
        perror("\n[ERRO] Arquivo nao encontrado.\n");
    }

    u_int64_t numVertices, numArestas;

    file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("\n[ERRO] Problema na leitura do arquivo\n");
    }

    lerQuantidadeVerticeArestas(&numVertices, &numArestas, file);

    Grafo *grafo = inicializaGrafoComVertice(numVertices, numArestas);

    construirGrafo(grafo, file);

    fclose(file);

    Grafo *agm = arvoreGeradoraMinima(*grafo);

    printf("Grafo AGM:\n");
    printGrafo(*agm);


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
