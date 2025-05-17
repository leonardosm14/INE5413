from grafo import Grafo
import sys

def Prim(G: Grafo):
    if G.qtdVertices() == 0:
        return []

    r = G.getVertices()[0]
    n = G.qtdVertices()

    predecessores = [None] * n
    chaves = [float('inf')] * n
    chaves[r.indice] = 0
    na_arvore = [False] * n

    for i in range(n):
        u = None
        min_chave = float('inf')
        for v in G.getVertices():
            if not na_arvore[v.indice] and chaves[v.indice] < min_chave:
                min_chave = chaves[v.indice]
                u = v

        if u is None:
            break

        na_arvore[u.indice] = True

        for v in G.vizinhos(u):
            peso = G.peso(u, v)
            if not na_arvore[v.indice] and peso < chaves[v.indice]:
                predecessores[v.indice] = u
                chaves[v.indice] = peso

    return predecessores

def main():
    if len(sys.argv) != 2:
        print("Uso: python arvore_geradora_minima.py <arquivo_grafo>", file=sys.stderr)
        sys.exit(1)

    try:
        grafo = Grafo(dirigido=False)
        grafo.ler(sys.argv[1])

        predecessores = Prim(grafo)

        peso_total = 0.0
        arestas_agm = []

        for i in range(grafo.qtdVertices()):
            if predecessores[i] is not None:
                u = predecessores[i]
                v = grafo.buscarVerticePorIndice(i)
                peso = grafo.peso(u, v)
                peso_total += peso
                
                indice_u = u.indice
                indice_v = v.indice

                arestas_agm.append(f"{indice_u+1}-{indice_v+1}")

        arestas_agm.sort()

        print(f"{peso_total:.1f}")
        print(','.join(arestas_agm))

    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()