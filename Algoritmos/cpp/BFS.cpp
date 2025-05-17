#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include "../Grafos - Tipos/grafo.hpp"

using namespace std;

template <typename T>
pair<vector<int>, vector<Vertice<T>*>> BFS(const Grafo<T>& G, const Vertice<T>& s) {
    int n = G.getQuantidadeVertices();
    vector<bool> C(n, false);
    vector<int> D(n, numeric_limits<int>::max());
    vector<Vertice<T>*> A(n, nullptr);
    queue<Vertice<T>*> Q;

    int origem = G.getIndiceDoVertice(s);
    C[origem] = true;
    D[origem] = 0;
    Q.push(&G.getVertice(origem));

    while (!Q.empty()) {
        Vertice<T>* u = Q.front(); 
        Q.pop();
        for (Vertice<T> v : G.getVizinhos(*u)) {
            int indexV = G.getIndiceDoVertice(v);
            if (!C[indexV]) {
                C[indexV] = true;
                D[indexV] = D[G.getIndiceDoVertice(*u)] + 1;
                A[indexV] = u;
                Q.push(&G.getVertice(indexV));
            }
        }
    }

    return {D, A};
}


// TESTE

int main() {
    
    Grafo<char> grafo(5);
    grafo.adicionarVertice('A');
    grafo.adicionarVertice('B');
    grafo.adicionarVertice('C');
    grafo.adicionarVertice('D');
    grafo.adicionarVertice('E');

    grafo.adicionarAresta(0, 1, 1);
    grafo.adicionarAresta(0, 2, 1);
    grafo.adicionarAresta(1, 3, 1);
    grafo.adicionarAresta(2, 4, 1);

    grafo.imprimir();

    auto [distancias, predecessores] = BFS(grafo, grafo.getVertice(0));

    cout << "\nDistâncias a partir de A:\n";
    for (int i = 0; i < distancias.size(); ++i) {
        cout << grafo.getVertice(i).informacao << ": " << distancias[i] << endl;
    }

    return 0;
}
