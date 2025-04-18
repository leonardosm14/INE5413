#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include "A1_1.h"

using namespace std;

template <typename T>
vector<vector<double>> FloydWarshall(const Grafo<T>& G) {
    int n = G.getQuantidadeVertices();
    const double INF = numeric_limits<double>::infinity();
    
    vector<vector<double>> D(n, vector<double>(n, INF));

    // Inicialização
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

void imprimirDistancias(const vector<vector<double>>& D) {
    int n = D.size();
    for (int i = 0; i < n; i++) {
        cout << (i) << ":";
        for (int j = 0; j < n; j++) {
            if (D[i][j] == numeric_limits<double>::infinity()) {
                cout << "INF";
            } else {
                cout << D[i][j];
            }
            if (j < n-1) cout << ",";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada>" << endl;
        return -1;
    }

    ifstream entrada(argv[1]);
    if (!entrada.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
        return 1;
    }

    Grafo<string> g;
    g.ler(entrada);
    
    auto D = FloydWarshall(g);
    imprimirDistancias(D);

    return 0;
}