import queue
from typing import Dict, List, Union
from grafo import Grafo, Vertice, Aresta
from itertools import chain, combinations

def conjuntoPotencia(V: list):
    "Retorna o conjunto potência de um iterável."
    return chain.from_iterable(combinations(V, r) for r in range(len(V) + 1))

def conjuntosIndependentesMaximais(G: Grafo):
    V = G.getVertices()
    S = list(conjuntoPotencia(V))
    R = list()

    for X in S:
        c = True
        for v in X:
            for u in X:
                if G.haAresta(u, v):  # Verifica se há aresta entre dois vértices do conjunto
                    c = False
                    break
            if not c:
                break

        if c:
            # Verifica se X é maximal (não pode ser expandido sem perder independência)
            maximal = True
            for v in V:
                if v not in X:
                    if all(not G.haAresta(v, u) for u in X):  # v pode ser adicionado sem conflitos
                        maximal = False
                        break
            if maximal:
                R.append(X)

    return R

def Lawler(G: Grafo):
    V = G.getVertices()
    conjuntos_independentes = conjuntosIndependentesMaximais(G)  # Obtém os conjuntos independentes maximais
    X = [float('inf') for _ in range(pow(2, len(V)))]  # Inicializa X com infinito
    X[0] = 0  # O conjunto vazio tem custo 0

    for s in conjuntos_independentes:
        subset_index = sum(1 << V.index(v) for v in s)  # Calcula o índice do subconjunto
        for v in s:
            prev_index = subset_index & ~(1 << V.index(v))  # Remove v do subconjunto
            X[subset_index] = min(X[subset_index], X[prev_index] + 1)

    return X[-1]  # Retorna o número mínimo de cores necessárias