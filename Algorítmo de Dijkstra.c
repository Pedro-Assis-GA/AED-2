#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 5 // Número de vértices no grafo

// Função para encontrar o vértice com a menor distância
int encontrar_menor_distancia(int distancias[], int visitados[]) {
    int minimo = INT_MAX, indice_minimo;

    for (int v = 0; v < V; v++) {
        if (visitados[v] == 0 && distancias[v] <= minimo) {
            minimo = distancias[v];
            indice_minimo = v;
        }
    }
    return indice_minimo;
}

// Implementação do Algoritmo de Dijkstra
void dijkstra(int grafo[V][V], int origem) {
    int distancias[V]; 
    int visitados[V];  

    // Inicializa todas as distâncias como infinitas e visitados como falso
    for (int i = 0; i < V; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = 0;
    }

    distancias[origem] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = encontrar_menor_distancia(distancias, visitados);
        visitados[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!visitados[v] && grafo[u][v] && distancias[u] != INT_MAX 
                && distancias[u] + grafo[u][v] < distancias[v]) {
                distancias[v] = distancias[u] + grafo[u][v];
            }
        }
    }

    // Imprime o resultado
    printf("Vértice \t Distância da Origem (%d)\n", origem);
    for (int i = 0; i < V; i++) {
        printf("%d \t\t %d\n", i, distancias[i]);
    }
}

int main() {
    // Representação do grafo por Matriz de Adjacência
    int grafo[V][V] = {
        {0, 4, 0, 0, 0},
        {4, 0, 8, 0, 0},
        {0, 8, 0, 7, 0},
        {0, 0, 7, 0, 9},
        {0, 0, 0, 9, 0}
    };

    dijkstra(grafo, 0);

    return 0;
}
