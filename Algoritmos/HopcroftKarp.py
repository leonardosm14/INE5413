import queue
from typing import Dict, List, Union
from grafo import Grafo, Vertice, Aresta

global null

def HopcroftKarp(G: Grafo):
    X, Y = G.getX(), G.getY()

    V = G.getVertices()
    D = {v: float['inf'] for v in V}
    null = Vertice(None, null, None)
    mate = {v: null for v in V}

    m = 0

    while BFS(G, mate, D): 
        for x in X:
            if mate[x] is None:
                if DFS(G, mate, x, D):
                    m += 1
    
    return (m, mate)

def BFS(G: Grafo, mate: Dict[Vertice, Vertice], D: Dict[Vertice, float]):
    X, Y = G.getX(), G.getY()
    Q = queue.Queue()

    for x in X:
        if mate[x] == null:
            D[x] = 0
            Q.put(x)
        else:
            D[x] = float['inf']
    
    D[null] = float['inf']

    while not Q.empty():
        x = Q.get()
        if D[x] < D[null]:
            for y in G.vizinhos(x):
                if D[mate[y]] == float['inf']:
                    D[mate[y]] = D[x] + 1
                    Q.put(mate[y])
    
    return D[null] != float['inf']

def DFS(G: Grafo, mate: Dict[Vertice, Vertice], x: Vertice, D: Dict[Vertice, float]):
    X, Y = G.getX(), G.getY()
    if x!= null:
        for y in G.vizinhos(x):
            if D[mate[y]] == D[x] + 1:
                if DFS(G, mate, mate[y], D):
                    mate[y] = X
                    mate[x] = y
                    return True
        D[x] = float['inf']
        return False
    return True