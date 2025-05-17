from grafo import Grafo, Vertice, Aresta

def DFS_Visit_OT(G: Grafo, v: Vertice, C: list, T: list, F: list, tempo: int, O: list):
    C[v.indice] = True
    tempo += 1
    T[v.indice] = tempo

    for u in G.vizinhos(v=v):
        if not C[u.indice]:
            DFS_Visit_OT(G=G, v=u, C=C, T=T, F=F, tempo=tempo, O=O)
        
    tempo += 1
    F[v.indice] = tempo
    O.append(v)

def DFS_OT(G: Grafo):
    n = G.qtdVertices()
    C = [False] * n 
    T = [float('inf')] * n
    F = [float('inf')] * n
    tempo = 0
    O = list()

    for u in G.getVertices():
        if not C[u.indice]:
            DFS_Visit_OT(G=G, v=u, C=C, T=T, F=T, tempo=tempo, O=O)
    
    return O