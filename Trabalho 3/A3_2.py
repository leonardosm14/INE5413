import queue, sys
from typing import Dict
from grafo import Grafo, Vertice

null = Vertice(indice=-1, rotulo="null", vizinhos=[])

def HopcroftKarp(G: Grafo):
    X, Y = G.getX(), G.getY()

    V = G.getVertices()
    D = {v: float('inf') for v in V}
    mate = {v: null for v in V}

    m = 0

    while BFS(G, mate, D): 
        for x in X:
            if mate[x] == null:
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
            D[x] = float('inf')
    
    D[null] = float('inf')

    while not Q.empty():
        x = Q.get()
        if D[x] < D[null]:
            for y in G.vizinhos(x):
                if D[mate[y]] == float('inf'):
                    D[mate[y]] = D[x] + 1
                    Q.put(mate[y])
    
    return D[null] != float('inf')

def DFS(G: Grafo, mate: Dict[Vertice, Vertice], x: Vertice, D: Dict[Vertice, float]):
    if x != null:
        for y in G.vizinhos(x):
            if D[mate[y]] == D[x] + 1:
                if DFS(G, mate, mate[y], D):
                    mate[y] = x
                    mate[x] = y
                    return True
        D[x] = float('inf')
        return False
    return True

def main():
    if len(sys.argv) != 2:
        print("Uso: python A3_2.py <arquivo_grafo>", file=sys.stderr)
        sys.exit(1)
    
    try:
        G = Grafo(dirigido=False, bipartido=True)
        G.ler(sys.argv[1])
        
        m, mate = HopcroftKarp(G)
        print(m)
        
        # Formata as arestas do emparelhamento máximo
        print(", ".join(f"{x.rotulo}-{mate[x].rotulo}" for x in G.getX() if mate[x] != null))
    
    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()