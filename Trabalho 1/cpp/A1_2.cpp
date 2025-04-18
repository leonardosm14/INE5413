#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <limits>
#include <map>
#include "A1_1.h"

using namespace std;

map<int, vector<string>> BFS(const Grafo<string>& G, size_t indiceOrigem) {
    size_t n = G.qtdVertices();
    vector<bool> visitado(n, false);
    vector<int> nivel(n, -1);
    queue<size_t> fila;

    if (indiceOrigem >= n) {
        throw out_of_range("Índice de vértice inválido.");
    }

    visitado[indiceOrigem] = true;
    nivel[indiceOrigem] = 0;
    fila.push(indiceOrigem);

    while (!fila.empty()) {
        size_t u = fila.front();
        fila.pop();
        
        const Vertice<string>& verticeU = G.getVertice(u);
        for (size_t indiceVizinho : verticeU.vizinhos) {
            if (!visitado[indiceVizinho]) {
                visitado[indiceVizinho] = true;
                nivel[indiceVizinho] = nivel[u] + 1;
                fila.push(indiceVizinho);
            }
        }
    }

    map<int, vector<string>> niveis;
    for (size_t i = 0; i < n; ++i) {
        if (nivel[i] != -1) {
            niveis[nivel[i]].push_back(G.getVertice(i).rotulo);
        }
    }
    return niveis;
}

void imprimirNiveis(const map<int, vector<string>>& niveis) {
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
        cerr << "Uso: " << argv[0] << " <arquivo_entrada> <indice_vertice_origem>" << endl;
        return 1;
    }

    ifstream entrada(argv[1]);
    if (!entrada.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
        return 1;
    }

    Grafo<string> g;
    try {
        g.ler(entrada);
        
        size_t indiceOrigem = stoul(argv[2]);
        map<int, vector<string>> niveis = BFS(g, indiceOrigem);
        
        cout << "BFS iniciado no vértice [" << indiceOrigem << "]: " 
             << g.getVertice(indiceOrigem).rotulo << endl;
        imprimirNiveis(niveis);

    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }

    return 0;
}