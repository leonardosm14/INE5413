#include <iostream>
#include <vector>
#include <stack>
#include <tuple>
#include <limits>
#include "../../Grafos - Tipos/grafo.hpp"

using namespace std;

template <typename T>
pair<vector<int>, vector<Vertice<T>*>> InicializacaoG(const Grafo<T>& G, const Vertice<T>& s) {
    //inicialização
    int n = G.getQuantidadeVertices();
    vector<int> D(n, numeric_limits<int>::max());
    vector<Vertice<T>*> A(n, nullptr);
    D[G.getIndiceDoVertice(s)] = 0;
    return {D, A};
}