from grafo import Grafo

def Prim(G: Grafo):
    r = G.getVertices()[0]
    n = G.qtdVertices()

    A = [None] * n 
    K = [float('inf')] * n
    K[r.indice] = 0

    Q = G.getVertices().copy()

    while Q:
        u = min(Q, key=lambda v: K[v.indice])
        Q.remove(u)

        for v in G.vizinhos(u):
            peso_uv = G.peso(u, v)
            if v in Q and peso_uv < K[v.indice]:
                A[v.indice] = u
                K[v.indice] = peso_uv

    return A