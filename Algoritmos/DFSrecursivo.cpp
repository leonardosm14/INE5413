#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <limits>
#include "../Grafos - Tipos/grafo.hpp"

using namespace std;

template <typename T>
void DFS_visit(
    const Grafo<T>& G,
    const Vertice<T>& v,
    vector<bool>& C,
    vector<int>& Tp,
    vector<Vertice<T>*>& A,
    vector<int>& F,
    int& tempo
) {
    
    int indexV = G.getIndiceDoVertice(v);
    C[indexV] = true;
    Tp[indexV] = ++tempo;

    for (Vertice<T> u : G.getVizinhos(v)) {
        int indexU = G.getIndiceDoVertice(u);
        if (!C[indexU]) {
            A[indexU] = &G.getVertice(indexV);
            DFS_visit(G, u, C, Tp, A, F, tempo);
        }
    }

    F[indexV] = ++tempo;

}

template <typename T>
tuple<vector<bool>, vector<int>, vector<Vertice<T>*>, vector<int>> DFS_caller(const Grafo<T>& G, const Vertice<T> s) {
    int n = G.getQuantidadeVertices();
    int tempo = 0;
    vector<bool> C(n, false);
    vector<int> Tp(n, numeric_limits<int>::max());
    vector<int> F(n, numeric_limits<int>::max());
    vector<Vertice<T>*> A(n, nullptr);

    DFS_visit(G, s, C, Tp, A, F, tempo);

    return {C, Tp, A, F};
}

// TESTE

int main() {
    Grafo<char> grafo(5, true); // grafo direcionado
    grafo.adicionarVertice('A'); // 0
    grafo.adicionarVertice('B'); // 1
    grafo.adicionarVertice('C'); // 2
    grafo.adicionarVertice('D'); // 3
    grafo.adicionarVertice('E'); // 4

    grafo.adicionarAresta(0, 1, 1);
    grafo.adicionarAresta(0, 2, 1);
    grafo.adicionarAresta(1, 3, 1);
    grafo.adicionarAresta(3, 4, 1);
    grafo.adicionarAresta(2, 4, 1);

    auto [visitado, descoberta, predecessores, finalizacao] = DFS_caller(grafo, grafo.getVertice(0));

    cout << "Descoberta:\n";
    for (int i = 0; i < descoberta.size(); ++i) {
        cout << grafo.getVertice(i).informacao << ": " << descoberta[i] << endl;
    }

    cout << "\nFinalização:\n";
    for (int i = 0; i < finalizacao.size(); ++i) {
        cout << grafo.getVertice(i).informacao << ": " << finalizacao[i] << endl;
    }

    cout << "\nPredecessores:\n";
    for (int i = 0; i < predecessores.size(); ++i) {
        cout << grafo.getVertice(i).informacao << ": ";
        if (predecessores[i])
            cout << predecessores[i]->informacao;
        else
            cout << "null";
        cout << endl;
    }

    return 0;
}
