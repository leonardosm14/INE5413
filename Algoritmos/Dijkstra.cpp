#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <limits>
#include "../Grafos - Tipos/grafo.hpp"
#include "auxiliares/InicializacaoG.hpp"
#include "auxiliares/Relaxamento.hpp"

using namespace std;

template <typename T>
pair<vector<int>, vector<Vertice<T>*>> Dijkstra(const Grafo<T>& G, const Vertice<T>& s) {
    int n = G.getQuantidadeVertices();
    auto [D, A] = InicializacaoG(G, s);
    vector<bool> C(n, false);

    while (true) {
        // Encontrar o vértice u com menor D[u] entre os não visitados
        int u = -1;
        int menorDistancia = numeric_limits<int>::max();
        for (int i = 0; i < n; ++i) {
            if (!C[i] && D[i] < menorDistancia) {
                menorDistancia = D[i];
                u = i;
            }
        }

        if (u == -1) 
            break; // Todos os vértices foram visitados ou inacessíveis

        C[u] = true;

        // Relaxa os vizinhos de u
        Aresta* atual = G.getListaAdjacencia(u);
        while (atual != nullptr) {
            int vIndex = atual->destino;
            if (!C[vIndex]) {
                Vertice<T>* origem = &G.getVertice(u);
                Vertice<T>* destino = &G.getVertice(vIndex);
                pair<Vertice<T>*, Vertice<T>*> parAresta = {origem, destino};
                Relaxamento(G, parAresta, A, D);
            }
            atual = atual->proxima;
        }
    }

    return {D, A};

}

// TESTE

int main() {
    Grafo<char> G(5, true); // Grafo direcionado com 5 vértices

    G.adicionarVertice('A'); // 0
    G.adicionarVertice('B'); // 1
    G.adicionarVertice('C'); // 2
    G.adicionarVertice('D'); // 3
    G.adicionarVertice('E'); // 4

    // Arestas com pesos positivos
    G.adicionarAresta(0, 1, 10);
    G.adicionarAresta(0, 3, 5);
    G.adicionarAresta(1, 2, 1);
    G.adicionarAresta(1, 3, 2);
    G.adicionarAresta(2, 4, 4);
    G.adicionarAresta(3, 1, 3);
    G.adicionarAresta(3, 2, 9);
    G.adicionarAresta(3, 4, 2);
    G.adicionarAresta(4, 0, 7);
    G.adicionarAresta(4, 2, 6);

    auto [D, A] = Dijkstra(G, G.getVertice(0)); // Origem: A

    cout << "Distâncias mínimas a partir de A:\n";
    for (int i = 0; i < D.size(); ++i) {
        cout << G.getVertice(i).informacao << ": " << D[i] << "\n";
    }

    return 0;
}
