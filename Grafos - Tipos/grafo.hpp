#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
using namespace std;

struct Aresta {
    int destino;
    double peso;
    Aresta* proxima;

    Aresta(int d, double p) : destino(d), peso(p), proxima(nullptr) {}
};

template <typename T>
struct Vertice {
    T informacao;
    Aresta* listaAdjacencia;

    Vertice(T info) : informacao(info), listaAdjacencia(nullptr) {}
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
    Grafo(int cap, bool eh_direcionado = true) {
        capacidade = cap;
        quantidade = 0;
        direcionado = eh_direcionado;
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

    // Adicionar vértice com informação genérica
    void adicionarVertice(T informacao) {
        if (quantidade < capacidade) {
            vertices[quantidade++] = new Vertice<T>(informacao);
        } else {
            cout << "Capacidade máxima atingida.\n";
        }
    }

    // Adicionar aresta entre vértices com peso
    void adicionarAresta(int origem, int destino, double peso) {
        if (origem >= quantidade || destino >= quantidade) {
            cout << "Índice de vértice inválido.\n";
            return;
        }

        // Aresta de origem para destino
        Aresta* nova = new Aresta(destino, peso);
        nova->proxima = vertices[origem]->listaAdjacencia;
        vertices[origem]->listaAdjacencia = nova;

        // Se não for direcionado, adiciona também a aresta oposta
        if (!direcionado) {
            Aresta* reversa = new Aresta(origem, peso);
            reversa->proxima = vertices[destino]->listaAdjacencia;
            vertices[destino]->listaAdjacencia = reversa;
        }
    }

    int getQuantidadeVertices() const {
        return quantidade;
    }

    int getQuantidadeArestas() const {
        int total = 0;
        for (int i = 0; i < quantidade; ++i) {
            Aresta* atual = vertices[i]->listaAdjacencia;
            while (atual != nullptr) {
                total++;
                atual = atual->proxima;
            }
        }
        // Em grafos não direcionados, cada aresta é contada duas vezes
        return direcionado ? total : total / 2;
    }

    vector<Vertice<T>> getVizinhos(const Vertice<T>& v) const {
        vector<Vertice<T>> vizinhos;
        int indice = getIndiceDoVertice(v);
        if (indice == -1) {
            cout << "Vértice não encontrado.\n";
            return vizinhos;
        }

        Aresta* atual = vertices[indice]->listaAdjacencia;
        while (atual != nullptr) {
            int destino = atual->destino;
            vizinhos.push_back(*vertices[destino]);  // Copia o vértice
            atual = atual->proxima;
        }

        return vizinhos;
    }

    Aresta* getListaAdjacencia(int i) const {
        if (i >= 0 && i < quantidade) {
            return vertices[i]->listaAdjacencia;
        } else {
            return nullptr;
        }
    }

    int getIndiceDoVertice(const Vertice<T>& vertice) const {
        for (int i = 0; i < quantidade; ++i) {
            if (vertices[i]->informacao == vertice.informacao) {
                return i;
            }
        }
        return -1; // Não encontrado
    }

    Vertice<T>& getVertice(int i) const {
        if (i >= 0 && i < quantidade) {
            return *vertices[i];
        }
        throw out_of_range("Índice de vértice inválido.");
    }

    // Imprimir lista de adjacência
    void imprimir() const {
        cout << "G = (V, A, w)\n";
        for (int i = 0; i < quantidade; i++) {
            cout << "Vértice " << i << " (" << vertices[i]->informacao << "): ";
            Aresta* atual = vertices[i]->listaAdjacencia;
            while (atual != nullptr) {
                cout << "→ [" << atual->destino << " (" 
                     << vertices[atual->destino]->informacao << "), peso = " 
                     << atual->peso << "] ";
                atual = atual->proxima;
            }
            cout << endl;
        }
    }
};

#endif