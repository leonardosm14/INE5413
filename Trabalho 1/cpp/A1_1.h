#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

using namespace std;

const double INF = numeric_limits<double>::infinity();

template <typename T>
struct Vertice {
    T rotulo;
    vector<size_t> vizinhos;  // Armazena índices dos vizinhos

    bool operator==(const Vertice<T>& outro) const {
        return rotulo == outro.rotulo;
    }
};

struct Aresta {
    size_t origem;    // Índice do vértice origem
    size_t destino;   // Índice do vértice destino
    double peso;
};

template <typename T>
class Grafo {
private:
    vector<Vertice<T>> vertices;
    vector<Aresta> arestas;
    unordered_map<size_t, size_t> indiceParaPosicao;  // Mapeia índice do arquivo para posição no vetor
    unordered_map<T, size_t> rotuloParaIndice;        // Mapeia rótulo para índice no vetor
    map<T, size_t> rotulo_para_indice;

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

    const T& rotulo(const Vertice<T>& v) const {
        return v.rotulo;
    }

    bool haAresta(size_t origem, size_t destino) const {
        for (const Aresta& a : arestas) {
            if (a.origem == origem && a.destino == destino) {
                return true;
            }
        }
        return false;
    }

    double peso(size_t origem, size_t destino) const {
        for (const Aresta& a : arestas) {
            if (a.origem == origem && a.destino == destino) {
                return a.peso;
            }
        }
        return -1;
    }

    void adicionarVertice(size_t indice, const T& rotulo) {
        vertices[indice] = rotulo;
        rotulo_para_indice[rotulo] = indice;
    

        Vertice<T> v;
        v.rotulo = rotulo;
        vertices.push_back(v);
        size_t posicao = vertices.size() - 1;
        indiceParaPosicao[indice] = posicao;
        rotuloParaIndice[rotulo] = posicao;
    }

    void adicionarAresta(size_t origem, size_t destino, double peso) {
        if (indiceParaPosicao.find(origem) == indiceParaPosicao.end() || 
            indiceParaPosicao.find(destino) == indiceParaPosicao.end()) {
            throw runtime_error("Vértice de origem ou destino não encontrado.");
        }

        size_t posOrigem = indiceParaPosicao[origem];
        size_t posDestino = indiceParaPosicao[destino];
        
        vertices[posOrigem].vizinhos.push_back(posDestino);
        
        Aresta novaAresta;
        novaAresta.origem = posOrigem;
        novaAresta.destino = posDestino;
        novaAresta.peso = peso;
        arestas.push_back(novaAresta);
    }

    const vector<size_t>& getVizinhos(size_t indiceVertice) const {
        if (indiceVertice >= vertices.size()) {
            throw out_of_range("Índice de vértice inválido.");
        }
        return vertices[indiceVertice].vizinhos;
    }

    const Vertice<T>& getVertice(size_t indiceVertice) const {
        if (indiceVertice >= vertices.size()) {
            throw out_of_range("Índice de vértice inválido.");
        }
        return vertices[indiceVertice];
    }
    
    size_t getIndiceDoVertice(const T& rotulo) const {
        auto it = rotuloParaIndice.find(rotulo);
        if (it == rotuloParaIndice.end()) {
            throw runtime_error("Rótulo de vértice não encontrado: " + rotulo);
        }
        return it->second;
    }

    void ler(ifstream& arquivo) {
        string linha;
        bool lendoVertices = false, lendoArestas = false;
        size_t n = 0;

        while (getline(arquivo, linha)) {
            // Ignora linhas vazias e comentários
            if (linha.empty() || linha[0] == '#') continue;

            // Verifica se é a linha de cabeçalho
            if (linha.find("*vertices") != string::npos) {
                lendoVertices = true;
                lendoArestas = false;
                
                // Extrai o número de vértices
                stringstream ss(linha);
                string token;
                ss >> token >> n;
                continue;
            } 
            else if (linha.find("*edges") != string::npos) {
                lendoVertices = false;
                lendoArestas = true;
                continue;
            }

            if (lendoVertices) {
                stringstream ss(linha);
                size_t indice;
                T rotulo;
                
                ss >> indice;
                // Lê o restante da linha como rótulo
                getline(ss, rotulo);
                
                // Remove espaços em branco no início e fim
                rotulo.erase(0, rotulo.find_first_not_of(" \t"));
                rotulo.erase(rotulo.find_last_not_of(" \t") + 1);
                
                // Remove aspas se existirem
                if (rotulo.size() >= 2 && rotulo.front() == '"' && rotulo.back() == '"') {
                    rotulo = rotulo.substr(1, rotulo.size() - 2);
                }
                
                adicionarVertice(indice, rotulo);
            } 
            else if (lendoArestas) {
                string rotuloOrigem, rotuloDestino;
                double peso;
            
                stringstream ss(linha);
                ss >> quoted(rotuloOrigem) >> quoted(rotuloDestino) >> peso;
            
                // Mapeia os rótulos para índices
                size_t origem = rotulo_para_indice[rotuloOrigem];
                size_t destino = rotulo_para_indice[rotuloDestino];
            
                adicionarAresta(origem, destino, peso);
            }
        }
    }

};

#endif