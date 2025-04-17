// LEONARDO DE SOUSA MARQUES & THAYSE ESTEVO TEIXEIRA - 2025
#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <stdexcept>

using namespace std;

const double INF = numeric_limits<double>::infinity();

struct Aresta {
    int destino;
    double peso;
    Aresta* proxima;

    Aresta(int d, double p) : destino(d), peso(p), proxima(nullptr) {}
};

template <typename T>
struct Vertice {
    T rotulo;
    Aresta* listaAdjacencia;

    Vertice(T rot) : rotulo(rot), listaAdjacencia(nullptr) {}
};

template <typename T>
class Grafo {
private:
    Vertice<T>** vertices;
    int quantidade;
    int capacidade;
    bool direcionado;

public:
    // Construtor
    Grafo(int cap, bool eh_direcionado = false) : capacidade(cap), quantidade(0), 
                                                direcionado(eh_direcionado) {
        vertices = new Vertice<T>*[capacidade];
    }

    // Destrutor
    ~Grafo() {
        for (int i = 0; i < quantidade; i++) {
            Aresta* atual = vertices[i]->listaAdjacencia;
            while (atual != nullptr) {
                Aresta* temp = atual;
                atual = atual->proxima;
                delete temp;
            }
            delete vertices[i];
        }
        delete[] vertices;
    }

    // Métodos básicos solicitados
    int qtdVertices() const {
        return quantidade;
    }

    int qtdArestas() const {
        int total = 0;
        for (int i = 0; i < quantidade; ++i) {
            Aresta* atual = vertices[i]->listaAdjacencia;
            while (atual != nullptr) {
                total++;
                atual = atual->proxima;
            }
        }
        return direcionado ? total : total / 2;
    }

    int grau(Vertice<T>* v) const {
        if (!pertence(v)) {
            throw invalid_argument("Vértice não pertence ao grafo");
        }
        
        int grau = 0;
        Aresta* atual = v->listaAdjacencia;
        while (atual != nullptr) {
            grau++;
            atual = atual->proxima;
        }
        return grau;
    }

    T rotulo(Vertice<T> v) const {
        return v.rotulo;
    }

    vector<Vertice<T>*> vizinhos(Vertice<T>* v) const {
        if (!pertence(v)) {
            throw invalid_argument("Vértice não pertence ao grafo");
        }
        
        vector<Vertice<T>*> vizinhos;
        Aresta* atual = v->listaAdjacencia;
        while (atual != nullptr) {
            vizinhos.push_back(atual->destino);
            atual = atual->proxima;
        }
        return vizinhos;
    }

    bool haAresta(int u, int v) const {
        if (u < 0 || u >= quantidade || v < 0 || v >= quantidade) {
            return false;
        }
        
        Aresta* atual = vertices[u]->listaAdjacencia;
        while (atual != nullptr) {
            if (atual->destino == v) {
                return true;
            }
            atual = atual->proxima;
        }
        return false;
    }

    double peso(Vertice<T>* u, Vertice<T>* v) const {
        int indexU = getIndiceDoVertice(*u);
        int indexV = getIndiceDoVertice(*v);
    
        if (indexU == -1 || indexV == -1) {
            throw invalid_argument("Vértice não encontrado.");
        }
    
        Aresta* atual = vertices[indexU]->listaAdjacencia;
        while (atual != nullptr) {
            if (atual->destino == indexV) {
                return atual->peso;
            }
            atual = atual->proxima;
        }
    
        throw runtime_error("Aresta não encontrada entre os vértices.");
    }

    // Métodos auxiliares para construção do grafo
    Vertice<T>* adicionarVertice(T rotulo) {
        if (mapaRotulos.find(rotulo) != mapaRotulos.end()) {
            throw invalid_argument("Vértice com este rótulo já existe");
        }
        
        Vertice<T>* novo = new Vertice<T>(rotulo);
        vertices.push_back(novo);
        mapaRotulos[rotulo] = novo;
        return novo;
    }

    void adicionarAresta(Vertice<T>* origem, Vertice<T>* destino, double peso) {
        if (!pertence(origem) || !pertence(destino)) {
            throw invalid_argument("Vértice não pertence ao grafo");
        }

        // Adiciona aresta de origem para destino
        Aresta* nova = new Aresta(destino, peso);
        nova->proxima = origem->listaAdjacencia;
        origem->listaAdjacencia = nova;

        // Se não for direcionado, adiciona a aresta inversa
        if (!direcionado && origem != destino) {
            Aresta* inversa = new Aresta(origem, peso);
            inversa->proxima = destino->listaAdjacencia;
            destino->listaAdjacencia = inversa;
        }
    }

    // Função para ler de arquivo
    void ler(istream& arquivo) {
        string linha;
        
        // Lê cabeçalho
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            if (linha.find("*vertices") == 0) break;
        }
        
        // Lê quantidade de vértices
        stringstream ss(linha);
        string token;
        int n;
        ss >> token >> n;
        
        // Lê vértices
        for (int i = 0; i < n; i++) {
            getline(arquivo, linha);
            stringstream ss_vert(linha);
            int indice;
            T rotulo;
            ss_vert >> indice >> rotulo;
            adicionarVertice(rotulo);
        }
        
        // Procura seção de arestas
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            if (linha.find("*edges") == 0) break;
        }
        
        // Lê arestas
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            
            stringstream ss_aresta(linha);
            int u, v;
            double peso = 1.0;
            
            if (ss_aresta >> u >> v) {
                ss_aresta >> peso;
                adicionarAresta(u-1, v-1, peso);
            }
        }
    }
};

#endif