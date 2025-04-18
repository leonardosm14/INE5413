#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <limits>
#include <map>
#include "A1_1.h"

using namespace std;

template <typename T>
map<int, vector<T>> BFS(const Grafo<T>& G, const T& rotuloOrigem) {
    size_t n = G.qtdVertices();
    vector<bool> visitado(n, false);
    vector<int> nivel(n, -1);
    queue<size_t> fila;

    // Obter índice do vértice de origem
    size_t origem = G.getIndiceDoVertice(rotuloOrigem);

    visitado[origem] = true;
    nivel[origem] = 0;
    fila.push(origem);

    while (!fila.empty()) {
        size_t u = fila.front();
        fila.pop();
        
        // Obter vizinhos do vértice u
        const Vertice<T>& verticeU = G.getVertice(u);
        for (const T& rotuloVizinho : verticeU.vizinhos) {
            size_t v = G.getIndiceDoVertice(rotuloVizinho);
            if (!visitado[v]) {
                visitado[v] = true;
                nivel[v] = nivel[u] + 1;
                fila.push(v);
            }
        }
    }

    // Agrupar vértices por nível
    map<int, vector<T>> niveis;
    for (size_t i = 0; i < n; ++i) {
        if (nivel[i] != -1) {
            niveis[nivel[i]].push_back(G.getVertice(i).rotulo);
        }
    }

    return niveis;
}

template <typename T>
void imprimirNiveis(const map<int, vector<T>>& niveis) {
    for (const auto& par : niveis) {
        cout << par.first << ": ";
        for (size_t i = 0; i < par.second.size(); ++i) {
            cout << par.second[i];
            if (i < par.second.size() - 1) cout << ",";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada> <vertice_origem>" << endl;
        return 1;
    }

    ifstream entrada(argv[1]);
    if (!entrada.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
        return 1;
    }

    Grafo<string> g;
    g.ler(entrada);

    string rotuloOrigem = argv[2];
    try {
        map<int, vector<string>> niveis = BFS(g, rotuloOrigem);
        imprimirNiveis(niveis);
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}