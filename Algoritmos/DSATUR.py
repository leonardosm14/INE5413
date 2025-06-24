from typing import Dict, List, Union
from grafo import Grafo, Vertice, Aresta

# Método Heurístico DSATUR para coloração de vértices em um grafo.
def DSATUR(G: Grafo) -> Dict[Vertice, int]:
    V = G.getVertices()
    X = {v: 0 for v in V}  # Dicionário de cores atribuídas aos vértices.
    D = {v: 0 for v in V}  # Dicionário de saturação dos vértices.
    i = 0

    while i < len(V):
        # Seleciona o vértice a ser colorido.
        u = max(V, key=lambda v: (D[v], G.getGrau(v) if X[v] == 0 else -1))
        S = [v for v in V if X[v] == 0 and D[v] == D[u]]

        if len(S) > 1:
            w = max(S, key=lambda v: G.getGrau(v))
        else:
            w = S[0]
        u = w

        # Determina a menor cor disponível para o vértice u.
        cviz = [X[v] for v in G.vizinhos(u) if X[v] != 0]
        cand = [j for j in range(1, max(cviz, default=0) + 2) if j not in cviz]
        cor = min(cand)
        X[u] = cor

        # Atualiza a saturação dos vértices vizinhos.
        for v in G.vizinhos(u):
            if X[v] == 0:  # Apenas vértices não coloridos têm sua saturação atualizada.
                cviz1 = {X[w] for w in G.vizinhos(v) if X[w] != 0}
                D[v] = len(cviz1)
        
        i += 1
    
    return X