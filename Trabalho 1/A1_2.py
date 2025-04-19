import sys
from collections import defaultdict
from A1_1 import Grafo

def BFS(G: Grafo, indice_origem: int):
    n = G.qtdVertices()
    visitado = [False] * n
    nivel = [-1] * n
    fila = []

    visitado[indice_origem] = True
    nivel[indice_origem] = 0
    fila.append(indice_origem)

    while fila:
        u = fila.pop(0)
        vertice_u = G.buscarVerticePorIndice(u+1)
        for vizinho in vertice_u.vizinhos:
            v = G.getIndiceVertice(vizinho)-1
            if not visitado[v]:
                visitado[v] = True
                nivel[v] = nivel[u] + 1
                fila.append(v)

    niveis = defaultdict(list)
    for i in range(n):
        if nivel[i] != -1:
            niveis[nivel[i]].append(i+1)

    return niveis


def imprimir_niveis(niveis):
    for nivel in niveis:
        print(f"{nivel}: {','.join(map(str, niveis[nivel]))}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Uso: {sys.argv[0]} <arquivo_entrada> <vertice_origem>")
        sys.exit(1)

    try:
        g = Grafo()
        g.ler(sys.argv[1])

        indice_origem = int(sys.argv[2])-1
        niveis = BFS(g, indice_origem)
        imprimir_niveis(niveis)
    except Exception as e:
        print(f"Erro: {e}")
        sys.exit(1)
