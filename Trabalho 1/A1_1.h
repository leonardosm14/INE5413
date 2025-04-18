#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

const double INF = numeric_limits<double>::infinity();

template <typename T>
struct Vertice {
    T rotulo;
    vector<T> vizinhos;

    bool operator==(const Vertice<T>& outro) const {
        return rotulo == outro.rotulo;
    }
};

template <typename T>
struct Aresta {
    Vertice<T> origem;
    Vertice<T> destino;
    double peso;
};

template <typename T>
class Grafo {
private:
    vector<Vertice<T>> vertices;
    vector<Aresta<T>> arestas;

    Vertice<T>* encontrarVertice(const T& rotulo) {
        for (auto& v : vertices) {
            if (v.rotulo == rotulo) return &v;
        }
        return nullptr;
    }

public:
    Grafo() = default;

    size_t qtdVertices() const {
        return vertices.size();
    }

    size_t qtdArestas() const {
        return arestas.size();
    }

    size_t grau(const Vertice<T>& v) const {
        return v.vizinhos.size();
    }

    T& rotulo(const Vertice<T>& v) const {
        return v.rotulo;
    }

    bool haAresta(const Vertice<T>& u, const Vertice<T>& v) const {
        for (const Aresta<T>& a : arestas) {
            if (a.origem == u && a.destino == v) {
                return true;
            }
        }
        return false;
    }

    double peso(const Vertice<T>& u, const Vertice<T>& v) const {
        for (const Aresta<T>& a : arestas) {
            if (a.origem == u && a.destino == v) {
                return a.peso;
            }
        }
        return -1;
    }

    void adicionarVertice(const T& rotulo) {
        if (!encontrarVertice(rotulo)) {
            Vertice<T> v;
            v.rotulo = rotulo;
            vertices.push_back(v);
        }
    }

    void adicionarAresta(const T& origemRotulo, const T& destinoRotulo, double peso) {
        Vertice<T>* origem = encontrarVertice(origemRotulo);
        Vertice<T>* destino = encontrarVertice(destinoRotulo);

        if (!origem || !destino) {
            throw runtime_error("Vertice de origem ou destino nao encontrado.");
        }

        origem->vizinhos.push_back(destinoRotulo);
        arestas.push_back({ *origem, *destino, peso });
    }

    const vector<Vertice<T>>& getVizinhos(const Vertice<T>& v) {
        return v.vizinhos;
    }

    const Vertice<T>& getVertice(size_t indice) {
        if (indice >= vertices.size()) {
            throw out_of_range("Índice de vértice inválido.");
        }
        return vertices[indice];
    }
    
    const Vertice<T>& getVertice(size_t indice) const {
        if (indice >= vertices.size()) {
            throw out_of_range("Índice de vértice inválido.");
        }
        return vertices[indice];
    }
    
    size_t getIndiceDoVertice(const T& rotulo) const {
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (vertices[i].rotulo == rotulo) {
                return i;
            }
        }
        throw runtime_error("Rótulo de vértice não encontrado.");
    }
    

    void ler(ifstream& arquivo) {
        string linha;
        bool lendoVertices = false, lendoArestas = false;

        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;

            if (linha.find("*vertices") != string::npos) {
                lendoVertices = true;
                lendoArestas = false;
                continue;
            } else if (linha.find("*edges") != string::npos) {
                lendoVertices = false;
                lendoArestas = true;
                continue;
            }

            stringstream ss(linha);

            if (lendoVertices) {
                size_t id;
                T rotulo;
                ss >> id >> rotulo;
                adicionarVertice(rotulo);
            } else if (lendoArestas) {
                T origem, destino;
                double peso;
                ss >> origem >> destino >> peso;
                adicionarAresta(origem, destino, peso);
            }
        }
    }

};

#endif
