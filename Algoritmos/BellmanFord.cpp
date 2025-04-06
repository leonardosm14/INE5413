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
tuple<bool, vector<int>, vector<Vertice<T>*>> BellmanFord(const Grafo<T>& G, const Vertice<T>& s) {

    int n = G.getQuantidadeVertices();
    auto [D, A] = InicializacaoG(G, s);

    for (int u = 0; u < n; ++u) {
        Aresta* atual = G.getListaAdjacencia(u);
        while (atual != nullptr) {
            Vertice<T>* origem = &G.getVertice(u);
            Vertice<T>* destino = &G.getVertice(atual->destino);
            Relaxamento(G, {origem, destino}, A, D);
            atual = atual->proxima;
        }
    }
    

    for (int u = 0; u < n; ++u) {
        Aresta* atual = G.getListaAdjacencia(u);
        while (atual != nullptr) {
            int v = atual->destino;
            Vertice<T>* origem = &G.getVertice(u);
            Vertice<T>* destino = &G.getVertice(v);
            if (D[v] > D[u] + G.w(origem, destino)) {
                cout << "Grafo contém ciclo de peso negativo.\n";
                return {false, {}, {}};
            }
            atual = atual->proxima;
        }
    }

    return {true, D, A};

}

// TESTE

int main() {
    Grafo<string> G(5, true); // 5 vértices, direcionado

    G.adicionarVertice("A"); // índice 0
    G.adicionarVertice("B"); // 1
    G.adicionarVertice("C"); // 2
    G.adicionarVertice("D"); // 3
    G.adicionarVertice("E"); // 4

    // Adicionando arestas com pesos (algumas negativas)
    G.adicionarAresta(0, 1, 6);   // A → B
    G.adicionarAresta(0, 3, 7);   // A → D
    G.adicionarAresta(1, 2, 5);   // B → C
    G.adicionarAresta(1, 3, 8);   // B → D
    G.adicionarAresta(1, 4, 4);  // B → E
    G.adicionarAresta(2, 1, 2);  // C → B
    G.adicionarAresta(3, 2, 3);  // D → C
    G.adicionarAresta(3, 4, 9);   // D → E
    G.adicionarAresta(4, 0, 2);   // E → A
    G.adicionarAresta(4, 2, 7);   // E → C

    auto [ok, D, A] = BellmanFord(G, G.getVertice(0)); // origem: A

    if (ok) {
        cout << "Distâncias mínimas a partir de A:\n";
        for (int i = 0; i < D.size(); ++i) {
            cout << G.getVertice(i).informacao << ": " << D[i] << "\n";
        }
    }

    return 0;
}