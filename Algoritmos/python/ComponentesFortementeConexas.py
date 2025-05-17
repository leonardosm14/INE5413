from grafo import Grafo, Vertice, Aresta

def DFS_Visit(G: Grafo, v: Vertice, C: list, 
              T: list, A: list, F: list, tempo: int):
    
    C[v.indice] = True
    tempo += 1
    T[v.indice] = tempo

    for u in G.vizinhos(v):
        if not C[u.indice]:
            A[u.indice] = v
            DFS_Visit(G=G, v=u, C=C, T=T, A=A, F=F,tempo=tempo)

    tempo += 1
    F[v.indice] = tempo

def DFS(G: Grafo):
    n = G.qtdVertices()
    C = [False] * n 
    T = [float('inf')] * n
    F = [float('inf')] * n
    A = [None] * n
    tempo = 0

    for u in G.getVertices():
        if not C[u.indice]:
            DFS_Visit(G=G, v=u, C=C, T=T, A=A, F=F, tempo=tempo)
    
    return C, T, A, F

def CFC(G: Grafo):
    C, T, A, F = DFS(G=G)
    
    At = list()

    for a in G.getArestas():
        origem = a.origem
        destino = a.destino
        aresta_invertida = Aresta(origem=destino, destino=origem)
        At.append(aresta_invertida)
    
    Gt = Grafo(dirigido=True,
               vertices=G.getVertices(),
               arestas=At)
    
    Ct, Tt, Alt, Ft = DFS(G=Gt)

    return Alt