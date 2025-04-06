#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <limits>
#include "../Grafos - Tipos/grafo.hpp"

using namespace std;

template <typename T>
tuple<vector<bool>, vector<int>, vector<Vertice<T>*>> DFS(const Grafo<T>& G, const Vertice<T>& s) {
    int n = G.getQuantidadeVertices();
    int tempo = 0;

    vector<bool> C(n, false);
    vector<int> Tp(n, numeric_limits<int>::max());
    vector<Vertice<T>*> A(n, nullptr);
    stack<Vertice<T>*> S;

    int origem = G.getIndiceDoVertice(s);
    C[origem] = true;
    S.push(&G.getVertice(origem));

    while (!S.empty()) {
        tempo++;
        Vertice<T>* u = S.top();
        S.pop();
        Tp[G.getIndiceDoVertice(*u)] = tempo;

        for (Vertice<T> v : G.getVizinhos(*u)) {
            int indexV = G.getIndiceDoVertice(v);
            if (!C[indexV]) {
                C[indexV] = true;
                A[indexV] = u;
                S.push(&G.getVertice(indexV));
            } 
        }
    }

    return {C, Tp, A};
}

// TESTE

int main() {
    // Cria um grafo com 5 vértices, não direcionado
    Grafo<int> G(5, false);

    // Adiciona vértices com valores de 0 a 4
    for (int i = 0; i < 5; ++i)
        G.adicionarVertice(i);

    // Adiciona algumas arestas
    G.adicionarAresta(0, 1, 1.0);
    G.adicionarAresta(0, 2, 1.0);
    G.adicionarAresta(1, 3, 1.0);
    G.adicionarAresta(1, 4, 1.0);

    // Escolhe o vértice inicial
    Vertice<int> origem = G.getVertice(0);

    // Executa DFS
    auto [visitado, tempo, predecessores] = DFS(G, origem);

    // Mostra os resultados
    cout << "Visitados: ";
    for (bool v : visitado)
        cout << v << " ";
    cout << "\n";

    cout << "Tempo de visitação: ";
    for (int t : tempo)
        cout << t << " ";
    cout << "\n";

    cout << "Predecessores:\n";
    for (size_t i = 0; i < predecessores.size(); ++i) {
        cout << "Vértice " << i << ": ";
        if (predecessores[i])
            cout << predecessores[i]->informacao;
        else
            cout << "null";
        cout << "\n";
    }

    return 0;
}