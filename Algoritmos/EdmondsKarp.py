import queue
from typing import List, Union
from grafo import Grafo, Vertice, Aresta

def EdmondsKarp(G: Grafo, s: Vertice, t: Vertice, Gf: Grafo) -> Union[tuple[List[Vertice], int], None]:
    V = G.getVertices()

    C = {v: False for v in V}
    A = {v: None for v in V}
    C[s] = True

    Q = queue.Queue()
    Q.put(s)

    while not Q.empty():
        u = Q.get()
        for v in G.vizinhos(u):
            if (not C[v] and Gf.peso(u, v) > 0):  # a função de peso (w) é equivalente a de capacidade (c) neste caso.
                C[v] = True
                A[v] = u
                if (v == t):
                    # Construir o caminho aumentante P
                    p = [t]
                    w = t
                    while w != s:
                        w = A[w]
                        p.insert(0, w)

                    # Calcular o fluxo máximo F para o caminho P
                    f = float('inf')
                    for i in range(len(p) - 1):
                        f = min(f, Gf.peso(p[i], p[i + 1]))

                    return p, f
                Q.put(v)

    return None