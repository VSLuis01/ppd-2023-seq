#include "graphics/sdl_utils.h"

#define NUM_VERTICES 9


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

int main() {
    char nomeDoArquivo[] = "dados_entrada_sequencial.txt";
    int running = 1;

    Grafo *grafo = inicializaGrafoComVertice(NUM_VERTICES);

    construirGrafo(grafo, nomeDoArquivo);

    Grafo *agm = arvoreGeradoraMinima(grafo);

//    printGrafo(agm);
    /*sdlInitWindow(1600, 1300);

    while (running) {
        sdlClearRender();

        renderizarGrafo(grafo, desenharGrafo);

        sdlDraw();
        while (sdlPullEvent()) {
            if (sdlEventClosedWindow()) {
                sdlDestroyWindow();
                running = 0;
                break;
            }
        }
    }*/



//    printGrafo(grafo);

    deleteGrafo(grafo);
//    deleteGrafo(agm);

    return 0;
}
