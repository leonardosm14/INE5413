#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <limits>
#include "auxiliares/InicializacaoG.hpp"
#include "auxiliares/Relaxamento.hpp"
#include "../Grafos - Tipos/grafo.hpp"

using namespace std;
template <typename T>
vector<vector<int>> FloydWarshall(const Grafo<T>& G) {
    int n = G.getQuantidadeVertices();
    const int INF = numeric_limits<int>::max();

    vector<vector<int>> D(n, vector<int>(n, INF));

    for (int u = 0; u < n; ++u) {
        D[u][u] = 0;
        Aresta* atual = G.getListaAdjacencia(u);
        while (atual != nullptr) {
            D[u][atual->destino] = atual->peso;
            atual = atual->proxima;
        }
    }

    // Algoritmo de Floyd-Warshall
    for (int k = 0; k < n; ++k) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (D[u][k] != INF && D[k][v] != INF) {
                    D[u][v] = min(D[u][v], D[u][k] + D[k][v]);
                }
            }
        }
    }

    return D;
}


// TESTE

int main() {
    Grafo<string> G(5, true);

    G.adicionarVertice("A"); // índice 0
    G.adicionarVertice("B"); // 1
    G.adicionarVertice("C"); // 2
    G.adicionarVertice("D"); // 3
    G.adicionarVertice("E"); // 4

    G.adicionarAresta(0, 1, 6);   // A → B
    G.adicionarAresta(0, 3, 7);   // A → D
    G.adicionarAresta(1, 2, 5);   // B → C
    G.adicionarAresta(1, 3, 8);   // B → D
    G.adicionarAresta(1, 4, 4);   // B → E
    G.adicionarAresta(2, 1, 2);   // C → B
    G.adicionarAresta(3, 2, 3);   // D → C
    G.adicionarAresta(3, 4, 9);   // D → E
    G.adicionarAresta(4, 0, 2);   // E → A
    G.adicionarAresta(4, 2, 7);   // E → C

    auto D = FloydWarshall(G);

    cout << "Matriz de distâncias mínimas entre todos os pares:\n\n";

    int n = G.getQuantidadeVertices();
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (D[u][v] == numeric_limits<int>::max()) {
                cout << "INF\t";
            } else {
                cout << D[u][v] << "\t";
            }
        }
        cout << "\n";
    }

    return 0;
}