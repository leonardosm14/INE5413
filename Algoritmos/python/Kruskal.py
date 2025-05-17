from grafo import Grafo
from EdElement import EdElement

def Kruskal(G: Grafo):
    A = list()
    conjuntos = dict()
    uf = EdElement()

    for v in G.getVertices():
        conjuntos[v.indice] = EdElement()
        conjuntos[v.indice].pai = conjuntos[v.indice]

    E = sorted(G.getArestas(), key=lambda a: a.peso)

    for aresta in E:
        u = aresta.origem
        v = aresta.destino

        rep_u = uf.cdEncontra(conjuntos[u.indice])
        rep_v = uf.cdEncontra(conjuntos[v.indice])

        if rep_u != rep_v:
            A.append(aresta)
            uf.cdUniao(rep_u, rep_v)

    return A