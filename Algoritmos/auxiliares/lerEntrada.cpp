#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "../../Grafos - Tipos/grafo.hpp"

using namespace std;

template<typename T>
Grafo<T> lerArquivo(ifstream& arquivo) {
    string linha;
    int quantidadeVertices = 0;

    // Procurar linha com "*vertices n"
    while (getline(arquivo, linha)) {
        if (linha.find("*vertices") != string::npos) {
            stringstream ss(linha);
            string token;
            ss >> token >> quantidadeVertices;
            break;
        }
    }

    if (quantidadeVertices == 0) {
        throw runtime_error("Linha '*vertices n' não encontrada ou inválida.");
    }

    // Grafo não direcionado fixo
    Grafo<T> grafo(quantidadeVertices, false);

    // Ler vértices
    for (int i = 0; i < quantidadeVertices; i++) {
        if (!getline(arquivo, linha)) {
            throw runtime_error("Linhas de vértices insuficientes.");
        }

        stringstream ss_vert(linha);
        int indice;
        T rotulo;

        if (!(ss_vert >> indice >> rotulo)) {
            throw runtime_error("Formato inválido de vértice.");
        }

        grafo.adicionarVertice(rotulo);
    }

    // Ler arestas (até fim do arquivo)
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        stringstream ss_aresta(linha);
        T rotuloOrigem, rotuloDestino;
        double peso = 1.0;

        if (!(ss_aresta >> rotuloOrigem >> rotuloDestino)) {
            cerr << "Formato inválido de aresta: " << linha << endl;
            continue;
        }

        ss_aresta >> peso;

        Vertice<T> vOrigem(rotuloOrigem);
        Vertice<T> vDestino(rotuloDestino);

        int origem = grafo.getIndiceDoVertice(vOrigem);
        int destino = grafo.getIndiceDoVertice(vDestino);

        if (origem == -1 || destino == -1) {
            cerr << "Erro: vértice não encontrado (" 
                 << rotuloOrigem << " ou " << rotuloDestino << ")\n";
            continue;
        }

        grafo.adicionarAresta(origem, destino, peso);
    }

    return grafo;
}

// TESTE

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            cerr << "Uso: " << argv[0] << " <arquivo.txt>\n";
            return 1;
        }

        ifstream entrada(argv[1]);
        if (!entrada.is_open()) {
            cerr << "Erro ao abrir o arquivo: " << argv[1] << endl;
            return 1;
        }

        Grafo<string> g = lerArquivo<string>(entrada);

        cout << "\n=====================================\n";
        cout << "          GRAFO RESULTANTE           \n";
        cout << "=====================================\n\n";

        g.imprimir();

        cout << "\nVértices: " << g.getQuantidadeVertices() << endl;
        cout << "Arestas: " << g.getQuantidadeArestas() << endl;

        entrada.close();

    } catch (const exception& e) {
        cerr << "\nErro: " << e.what() << endl;
        return 1;
    }

    return 0;
}