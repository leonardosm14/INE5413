import queue
from typing import List, Union
from grafo import Grafo, Vertice, Aresta

def EdmondsKarp(G: Grafo, s: Vertice, t:Vertice, Gf: Grafo) -> Union[List[Vertice], None]:
    V = G.getVertices()

    C = {v: False for v in V}
    A = {v: None for v in V}
    C[s] = True

    Q = queue.Queue()
    Q.put(s)

    while not Q.empty():
        u = Q.get()
        for v in G.vizinhos(u):
            if (not C[v] and Gf.peso(u,v) > 0): #a função de peso (w) é equivalente a de capacidade (c) neste caso.
                C[v] = True
                A[v] = u
                if (v == t):
                    p = [t]
                    w = t
                    while (w != s):
                        w = A[w]
                        p.insert(0, w)
                    return p
                Q.put(v)
    
    return None