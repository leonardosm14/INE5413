import sys
from grafo import Grafo, Vertice, Aresta
from itertools import chain, combinations

def conjuntoPotencia(V: list):
    from itertools import chain, combinations
    return chain.from_iterable(combinations(V, r) for r in range(len(V) + 1))

def ConjuntoIndependente(G: Grafo, vertices: list):
    for i in range(len(vertices)):
        for j in range(i + 1, len(vertices)):
            if G.haAresta(vertices[i], vertices[j]):
                return False
    return True

def Lawler(G: Grafo):
    V = G.getVertices()
    n = len(V)
    id_map = {v: i for i, v in enumerate(V)}
    total = 1 << n
    X = [float('inf')] * total
    X[0] = 0
    trace = [-1] * total

    subconjuntos = conjuntoPotencia(V)
    for S in subconjuntos:
        if not ConjuntoIndependente(G, S):
            continue
        indice = sum(1 << id_map[v] for v in S)
        restante = ((1 << n) - 1) ^ indice
        sub = restante
        while True:
            combinado = indice | sub
            if X[combinado] > X[sub] + 1:
                X[combinado] = X[sub] + 1
                trace[combinado] = sub
            if sub == 0:
                break
            sub = (sub - 1) & restante

    coloracao = [0] * n
    atual = (1 << n) - 1
    cor = X[atual]
    while atual:
        anterior = trace[atual]
        diff = atual ^ anterior
        for i in range(n):
            if diff & (1 << i):
                coloracao[i] = cor
        cor -= 1
        atual = anterior

    return max(coloracao), coloracao

def main():
    if len(sys.argv) != 2:
        print("Uso: python A3_3.py <arquivo_grafo>", file=sys.stderr)
        sys.exit(1)
    
    try:
        G = Grafo(dirigido=False, bipartido=False)
        G.ler(sys.argv[1])
        
        num_cores, coloracao = Lawler(G=G)
        
        # Imprime o número de cores encontradas
        print(num_cores)
        
        # Imprime a cor correspondente a cada vértice
        print(", ".join(map(str, coloracao)))
    
    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()