#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <limits>
#include "../Grafos - Tipos/grafo.hpp"

using namespace std;

template <typename T>
pair<bool, vector<int>> buscarSubcicloEuleriano(const Grafo<T>& G, const Vertice<T>& v, vector<vector<int>>& C) {
    vector<int> ciclo;
    int t = G.getIndiceDoVertice(v);
    if (t == -1) return {false, {}};

    ciclo.push_back(t);  // ciclo <- (v)
    int atual = t;

    do {
        bool encontrou = false;
        Aresta* a = G.getListaAdjacencia(atual);

        while (a != nullptr) {
            int u = a->destino;
            if (C[atual][u] > 0) {
                C[atual][u]--;
                C[u][atual]--;
                atual = u;
                ciclo.push_back(u);  // ciclo <- ciclo · (v)
                encontrou = true;
                break;
            }
            a = a->proxima;
        }

        if (!encontrou) 
            return {false, {}};

    } while (atual != t);

    // busca por subciclos
    for (int i = 0; i < ciclo.size(); ++i) {
        int x = ciclo[i];
        Aresta* a = G.getListaAdjacencia(x);
        while (a != nullptr) {
            if (C[x][a->destino] > 0) {
                auto [ok, ciclo2] = buscarSubcicloEuleriano(G, G.getVertice(x), C);
                if (!ok) return {false, {}};

                ciclo.insert(ciclo.begin() + i + 1, ciclo2.begin() + 1, ciclo2.end());
                i += ciclo2.size() - 2;
            }
            a = a->proxima;
        }
    }

    return {true, ciclo};
}

template <typename T>
pair<bool, vector<int>> Hierholzer(const Grafo<T>& G) {
    int n = G.getQuantidadeVertices();
    vector<vector<int>> C(n, vector<int>(n, 0));

    for (int u = 0; u < n; ++u) {
        Aresta* atual = G.getListaAdjacencia(u);
        while (atual != nullptr) {
            int v = atual->destino;
            C[u][v] += 1;
            C[v][u] += 1; // como o grafo é não direcionado
            atual = atual->proxima;
        }
    }

    Vertice<T>& v = G.getVertice(0); // vértice arbitrário
    auto [r, ciclo] = buscarSubcicloEuleriano(G, v, C);

    if (!r) {
        return {false, {}};
    }

    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (C[u][v] > 0) {
                return {false, {}};
            }
        }
    }

    return {true, ciclo};
}

// TESTE

int main() {
    Grafo<char> grafo(5, false);
    grafo.adicionarVertice('A');
    grafo.adicionarVertice('B');
    grafo.adicionarVertice('C');
    grafo.adicionarVertice('D');
    grafo.adicionarVertice('E');

    grafo.adicionarAresta(0, 1, 1);
    grafo.adicionarAresta(1, 2, 1);
    grafo.adicionarAresta(2, 3, 1);
    grafo.adicionarAresta(3, 4, 1);
    grafo.adicionarAresta(4, 0, 1);
    grafo.adicionarAresta(1, 3, 1);

    grafo.imprimir();

    auto [existeCiclo, ciclo] = Hierholzer(grafo);

    if (existeCiclo) {
        cout << "\nCiclo Euleriano encontrado:\n";
        for (int i : ciclo) {
            cout << grafo.getVertice(i).informacao << " ";
        }
        cout << endl;
    } else {
        cout << "\nNão existe ciclo Euleriano no grafo.\n";
    }

    return 0;
}