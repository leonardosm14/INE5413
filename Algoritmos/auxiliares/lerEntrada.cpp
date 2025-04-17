#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../../Grafos - Tipos/grafo.hpp"

using namespace std;

template<typename T>
Grafo<T> lerEntradaTerminal(bool direcionado = false) {
    string linha;
    int quantidadeVertices = 0;
    
    // Primeiro pede o número de vértices
    getline(cin, linha);
    while (linha.find("*vertices") == string::npos) {
        cout << "Formato inválido. Use '*vertices n': ";
        getline(cin, linha);
    }
    
    stringstream ss(linha);
    string token;
    ss >> token >> quantidadeVertices;
    Grafo<T> grafo(quantidadeVertices, direcionado);

    // Ler vértices
    cout << "\nDigite os vértices (um por linha, formato: índice rótulo):\n";
    for (int i = 0; i < quantidadeVertices; i++) {
        while (true) {
            cout << "Vértice " << (i+1) << ": ";
            getline(cin, linha);
            
            stringstream ss_vert(linha);
            int indice;
            T rotulo;
            
            if (ss_vert >> indice >> rotulo) {
                grafo.adicionarVertice(rotulo);
                break;
            }
            cout << "Formato inválido. Use: índice rótulo\n";
        }
    }

    // Ler arestas
    
    while (true) {
        cout << "Aresta: ";
        getline(cin, linha);
        
        if (linha == "fim") break;
        if (linha.empty()) continue;
        
        stringstream ss_aresta(linha);
        T rotuloOrigem, rotuloDestino;
        double peso = 1.0; // Valor padrão
        
        if (!(ss_aresta >> rotuloOrigem >> rotuloDestino)) {
            cout << "Formato inválido. Use: origem destino [peso]\n";
            continue;
        }
        
        ss_aresta >> peso;
        
        // Buscar índices
        Vertice<T> vOrigem(rotuloOrigem);
        Vertice<T> vDestino(rotuloDestino);
        
        int origem = grafo.getIndiceDoVertice(vOrigem);
        int destino = grafo.getIndiceDoVertice(vDestino);
        
        if (origem == -1 || destino == -1) {
            cout << "Erro: Vértice não encontrado (" 
                 << rotuloOrigem << " ou " << rotuloDestino << ")\n";
            continue;
        }
        
        grafo.adicionarAresta(origem, destino, peso);
    }

    return grafo;
}

// TESTE

int main() {
    try {
        cout << "=====================================\n";
        cout << "   LEITURA INTERATIVA DE GRAFO      \n";
        cout << "=====================================\n\n";
        
        Grafo<string> g = lerEntradaTerminal<string>(false);
        
        cout << "\n=====================================\n";
        cout << "          GRAFO RESULTANTE           \n";
        cout << "=====================================\n\n";
        
        g.imprimir();
        
        cout << "\nVértices: " << g.getQuantidadeVertices() << endl;
        cout << "Arestas: " << g.getQuantidadeArestas() << endl;
        
    } catch (const exception& e) {
        cerr << "\nErro: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}