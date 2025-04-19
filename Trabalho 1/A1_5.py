import sys
from A1_1 import Grafo

INF = float('inf')

def floyd_warshall(G: Grafo):
    n = G.qtdVertices()

    D = [[INF] * n for _ in range(n)]

    for u in G.getArestas():
            D[u.origem.indice-1][u.destino.indice-1] = int(u.peso)
            D[u.destino.indice-1][u.origem.indice-1] = int(u.peso)

    # Algoritmo de Floyd-Warshall
    for k in range(n):
        for u in range(n):
            for v in range(n):
                if D[u][v] != INF:
                    if D[u][k] != INF and D[k][v] != INF:
                        D[u][v] = min(D[u][v], D[u][k] + D[k][v])
                else:
                    if D[u][k] != INF and D[k][v] != INF:
                        D[u][v] = D[u][k] + D[k][v]

    return D


if __name__ == "__main__":
    if len(sys.argv) != 2:
        
        print(f"Uso: {sys.argv[0]} <arquivo_grafo>")
        sys.exit(1)

    G = Grafo()
    G.ler(sys.argv[1])

    D = floyd_warshall(G)
    
    n = G.qtdVertices()
    for u in range(n):
        print(f"{u+1}:{','.join(map(str, D[u]))}\n")