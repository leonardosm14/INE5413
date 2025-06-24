import queue
from typing import List, Union
from grafo import Grafo, Vertice, Aresta

def EmparelhamentoMaximo(G: Grafo, s: Vertice, t: Vertice):
    V = G.getVertices() + [s, t]
    X, Y = G.getX(), G.getY() # Esses métodos ainda não existem, verificar se é necessário fazer.
    A = G.getArestas()

    for x in X:
        A += Aresta(s, x, 1)
    for y in Y:
        A += Aresta(y, t, 1)
    
    J = Grafo(dirigido=True, vertices=V, arestas=A)
    return J