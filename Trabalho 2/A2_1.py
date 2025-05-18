from grafo import Grafo, Vertice, Aresta
import sys

def DFS_visit(G, u, C, A=None, T=None, F=None, tempo=None, comp=None):
    C[u] = True
    if tempo is not None and T is not None:
        tempo[0] += 1
        T[u] = tempo[0]

    if comp is not None:
        comp.append(str(u + 1))

    v = G.buscarVerticePorIndice(u)
    for w in G.vizinhos(v):
        if not C[w.indice]:
            if A is not None:
                A[w.indice] = u
            DFS_visit(G, w.indice, C, A, T, F, tempo, comp)

    if tempo is not None and F is not None:
        tempo[0] += 1
        F[u] = tempo[0]

def DFS(G: Grafo):
    n = G.qtdVertices()
    C = [False] * n
    T = [float('inf')] * n
    F = [float('inf')] * n
    A = [None] * n
    tempo = [0]

    for v in G.vertices:
        if not C[v.indice]:
            DFS_visit(G, v.indice, C, A, T, F, tempo)

    return F, A

def DFS_adaptado(G: Grafo, ordem):
    n = G.qtdVertices()
    C = [False] * n
    A = [None] * n
    componentes = []

    for v in ordem:
        if not C[v.indice]:
            comp = []
            DFS_visit(G, v.indice, C, A, comp=comp)
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
