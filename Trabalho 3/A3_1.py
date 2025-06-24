import queue, sys
from grafo import Grafo, Vertice

def EdmondsKarp(G: Grafo, s: Vertice, t: Vertice, Gf: Grafo):
    V = G.getVertices()
    C = {v: False for v in V}
    A = {v: None for v in V}
    C[s] = True

    Q = queue.Queue()
    Q.put(s)

    while not Q.empty():
        u = Q.get()
        for v in G.vizinhos(u):
            if not C[v] and Gf.peso(u, v) > 0:
                C[v] = True
                A[v] = u
                if v == t:
                    p = [t]
                    w = t
                    while w != s:
                        w = A[w]
                        p.insert(0, w)

                    f = float('inf')
                    for i in range(len(p) - 1):
                        f = min(f, Gf.peso(p[i], p[i + 1]))

                    return p, f
                Q.put(v)

    return None

def fluxoMaximo(G: Grafo, s: Vertice, t: Vertice) -> float:
    Gf = Grafo(dirigido=True)
    for v in G.getVertices():
        Gf.adicionarVertice(v.indice, v.rotulo)
    
    rotulo_para_vertice = {v.rotulo: v for v in Gf.getVertices()}

    for a in G.getArestas():
        u = rotulo_para_vertice[a.origem.rotulo]
        v = rotulo_para_vertice[a.destino.rotulo]
        Gf.adicionarAresta(u, v, a.peso)
        if not Gf.haAresta(v, u):
            Gf.adicionarAresta(v, u, 0)

    total = 0
    while True:
        res = EdmondsKarp(G, s, t, Gf)
        if not res:
            break
        caminho, f = res
        total += f
        for i in range(len(caminho) - 1):
            u = caminho[i]
            v = caminho[i + 1]
            cap = Gf.peso(u, v)
            Gf.atualizarPeso(u, v, cap - f)
            cap_rev = Gf.peso(v, u)
            Gf.atualizarPeso(v, u, cap_rev + f)

    return total



def main():
    if len(sys.argv) != 4:
        print("Uso: python A3_1.py <arquivo_grafo> <s> <t>", file=sys.stderr)
        sys.exit(1)

    try:
        G = Grafo(dirigido=True)
        G.ler(sys.argv[1])
        s = G.buscarVerticePorIndice(indice=int(sys.argv[2]))
        t = G.buscarVerticePorIndice(indice=int(sys.argv[3]))
            
        F = fluxoMaximo(G, s, t)
        print(int(F))

    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()