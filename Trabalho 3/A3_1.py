import queue, sys
from typing import List, Union
from grafo import Grafo, Vertice, Aresta

def EdmondsKarp(G: Grafo, s: Vertice, t: Vertice):
    V = G.getVertices()

    C = {v: False for v in V}
    A = {v: None for v in V}
    C[s] = True

    Q = queue.Queue()
    Q.put(s)

    while not Q.empty():
        u = Q.get()
        for v in G.vizinhos(u):
            if (not C[v] and G.peso(u, v) > 0):  # a função de peso (w) é equivalente a de capacidade (c) neste caso.
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
                        f = min(f, G.peso(p[i], p[i + 1]))

                    return f
                Q.put(v)

    return None

def main():
    if len(sys.argv) != 4:
        print("Uso: python A3_1 <arquivo_grafo> <s> <t>", file=sys.stderr)
        sys.exit(1)
    
    try:
        G = Grafo(dirigido=True)
        G.ler(sys.argv[1])
        s = G.getVertice(label=sys.argv[2])
        t = G.getVertice(label=sys.argv[3])
        
        F = EdmondsKarp(G=G, s=s, t=t)
        print(F)
        
    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()