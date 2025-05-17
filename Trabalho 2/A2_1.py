from grafo import Grafo, Vertice, Aresta
import sys
from math import inf

def DFS(G: Grafo):
    n = G.qtdVertices()
    C = [False] * n      # visitados
    T = [inf] * n        # tempo inicial
    F = [inf] * n        # tempo final
    A = [None] * n       # antecessores
    tempo = [0]

    def DFS_visit(u):
        C[u] = True
        tempo[0] += 1
        T[u] = tempo[0]
        v = G.buscarVerticePorIndice(u)

        for w in G.vizinhos(v):
            if not C[w.indice]:
                A[w.indice] = u
                DFS_visit(w.indice)

        tempo[0] += 1
        F[u] = tempo[0]

    for v in G.vertices:
        if not C[v.indice]:
            DFS_visit(v.indice)

    return F, A

def DFS_adaptado(G: Grafo, ordem):
    n = G.qtdVertices()
    C = [False] * n
    A = [None] * n

    componentes = []

    def DFS_visit(u, comp):
        C[u] = True
        comp.append(str(u + 1))
        v = G.buscarVerticePorIndice(u)

        for w in G.vizinhos(v):
            if not C[w.indice]:
                A[w.indice] = u
                DFS_visit(w.indice, comp)

    for v in ordem:
        if not C[v.indice]:
            comp = []
            DFS_visit(v.indice, comp)
            componentes.append(comp)

    return componentes

def CFC(G: Grafo):
    F, _ = DFS(G)

    GT = Grafo(dirigido=True)
    for v in G.vertices:
        GT.adicionarVertice(v.indice, v.rotulo)
    for a in G.arestas:
        origem = GT.buscarVerticePorIndice(a.destino.indice)
        destino = GT.buscarVerticePorIndice(a.origem.indice)
        GT.adicionarAresta(origem, destino, a.peso)

    ordem = sorted(G.vertices, key=lambda v: F[v.indice], reverse=True)
    componentes = DFS_adaptado(GT, ordem)

    return componentes

def main():
    if len(sys.argv) != 2:
        print("Uso: python cfc.py <arquivo_grafo>", file=sys.stderr)
        sys.exit(1)

    try:
        G = Grafo(dirigido=True)
        G.ler(sys.argv[1])
        comps = CFC(G)
        for comp in comps:
            print(','.join(comp))
    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
