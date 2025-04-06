#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <limits>
#include "../../Grafos - Tipos/grafo.hpp"

using namespace std;

template <typename T>
void Relaxamento(const Grafo<T>& G, const pair<Vertice<T>*, Vertice<T>*>& aresta, vector<Vertice<T>*>& A, vector<int>& D) {
    auto [u, v] = aresta;
    int indexU = G.getIndiceDoVertice(*u);
    int indexV = G.getIndiceDoVertice(*v);

    if (D[indexV] > D[indexU] + G.w(u,v)) {
        D[indexV] = D[indexU] + G.w(u,v);
        A[indexV] = u;
    }
}
