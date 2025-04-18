// LEONARDO DE SOUSA MARQUES & THAYSE ESTEVO TEIXEIRA - 2025
#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

const double INF = numeric_limits<double>::infinity();

template <typename T>
struct Vertice;

template <typename T>
struct Aresta {
    Vertice<T>* destino;
    double peso;
    Aresta* proxima;

    Aresta(Vertice<T>* d, double p) : destino(d), peso(p), proxima(nullptr) {}
};

template <typename T>
struct Vertice {
    T rotulo;
    Aresta<T>* listaAdjacencia;

    Vertice(T rot) : rotulo(rot), listaAdjacencia(nullptr) {}
};

template <typename T>
class Grafo {
private:
    vector<Vertice<T>*> vertices;
    unordered_map<T, Vertice<T>*> mapaRotulos;
    bool direcionado;

public:
    // Construtor sem capacidade fixa
    Grafo(bool eh_direcionado = false) : direcionado(eh_direcionado) {}

    // Destrutor
    ~Grafo() {
        for (auto vertice : vertices) {
            Aresta<T>* atual = vertice->listaAdjacencia;
            while (atual != nullptr) {
                Aresta<T>* temp = atual;
                atual = atual->proxima;
                delete temp;
            }
            delete vertice;
        }
    }

    // Métodos básicos
    int qtdVertices() const { return vertices.size(); }

    int qtdArestas() const {
        int total = 0;
        for (auto vertice : vertices) {
            Aresta<T>* atual = vertice->listaAdjacencia;
            while (atual != nullptr) {
                total++;
                atual = atual->proxima;
            }
        }
        return direcionado ? total : total / 2;
    }

    int grau(Vertice<T>* v) const {
        if (!pertence(v)) throw invalid_argument("Vértice não pertence ao grafo");
        
        int grau = 0;
        Aresta<T>* atual = v->listaAdjacencia;
        while (atual != nullptr) {
            grau++;
            atual = atual->proxima;
        }
        return grau;
    }

    T rotulo(Vertice<T>* v) const {
        if (!pertence(v)) throw invalid_argument("Vértice não pertence ao grafo");
        return v->rotulo;
    }

    vector<Vertice<T>*> vizinhos(Vertice<T>* v) const {
        if (!pertence(v)) throw invalid_argument("Vértice não pertence ao grafo");
        
        vector<Vertice<T>*> vizinhos;
        Aresta<T>* atual = v->listaAdjacencia;
        while (atual != nullptr) {
            vizinhos.push_back(atual->destino);
            atual = atual->proxima;
        }
        return vizinhos;
    }

    bool haAresta(Vertice<T>* u, Vertice<T>* v) const {
        if (!pertence(u) || !pertence(v)) return false;
        
        Aresta<T>* atual = u->listaAdjacencia;
        while (atual != nullptr) {
            if (atual->destino == v) return true;
            atual = atual->proxima;
        }
        return false;
    }

    double peso(Vertice<T>* u, Vertice<T>* v) const {
        if (!pertence(u) || !pertence(v)) return INF;
        
        Aresta<T>* atual = u->listaAdjacencia;
        while (atual != nullptr) {
            if (atual->destino == v) return atual->peso;
            atual = atual->proxima;
        }
        return INF;
    }

    // Função para ler de arquivo
    void ler(istream& arquivo) {
        string linha;
        
        // Pular até a seção de vértices
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            if (linha.find("*vertices") == 0) break;
        }
        
        // Ler vértices
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            if (linha.find("*edges") == 0) break;
            
            stringstream ss(linha);
            int indice;
            T rotulo;
            if (ss >> indice >> rotulo) {
                adicionarVertice(rotulo);
            }
        }
        
        // Ler arestas
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            
            stringstream ss(linha);
            T u, v;
            double peso = 1.0;
            if (ss >> u >> v) {
                ss >> peso;
                adicionarAresta(u, v, peso);
            }
        }
    }

    // Métodos auxiliares
    Vertice<T>* adicionarVertice(T rotulo) {
        if (mapaRotulos.find(rotulo) != mapaRotulos.end()) {
            return mapaRotulos[rotulo]; // Retorna existente se já houver
        }
        
        Vertice<T>* novo = new Vertice<T>(rotulo);
        vertices.push_back(novo);
        mapaRotulos[rotulo] = novo;
        return novo;
    }

    void adicionarAresta(T rotuloU, T rotuloV, double peso) {
        Vertice<T>* u = adicionarVertice(rotuloU);
        Vertice<T>* v = adicionarVertice(rotuloV);
        
        // Adiciona aresta de u para v
        Aresta<T>* nova = new Aresta<T>(v, peso);
        nova->proxima = u->listaAdjacencia;
        u->listaAdjacencia = nova;

        // Se não for direcionado, adiciona a aresta inversa
        if (!direcionado && u != v) {
            Aresta<T>* inversa = new Aresta<T>(u, peso);
            inversa->proxima = v->listaAdjacencia;
            v->listaAdjacencia = inversa;
        }
    }

    vector<Vertice<T>*> getVertices() const { 
        return vertices; 
    }

    bool pertence(Vertice<T>* v) const {
        for (auto vertice : vertices) {
            if (vertice == v) return true;
        }
        return false;
    }

    Vertice<T>* getVertice(T rotulo) const {
        auto it = mapaRotulos.find(rotulo);
        return it != mapaRotulos.end() ? it->second : nullptr;
    }

};

#endif