from typing import List
from A1_1 import Grafo, Aresta
import sys

INF = float('inf')

def BellmanFord(G: Grafo, s: int):
    D = [INF] * G.qtdVertices()
    A = [None] * G.qtdVertices()
    D[s] = 0
    
    # Relaxamento das arestas (considerando grafo não-direcionado)
    for _ in range(G.qtdVertices() - 1):
        for aresta in G.arestas:
            u = aresta.origem.indice - 1  # 0-based
            v = aresta.destino.indice - 1
            
            # Relaxa em ambas as direções
            if D[u] != INF and D[u] + aresta.peso < D[v]:
                D[v] = D[u] + aresta.peso
                A[v] = u
                
            if D[v] != INF and D[v] + aresta.peso < D[u]:
                D[u] = D[v] + aresta.peso
                A[u] = v
                
    # Verificação de ciclo negativo (também em ambas as direções)
    for aresta in G.arestas:
        u = aresta.origem.indice - 1
        v = aresta.destino.indice - 1
        
        if D[u] != INF and D[u] + aresta.peso < D[v]:
            return False, D, A
            
        if D[v] != INF and D[v] + aresta.peso < D[u]:
            return False, D, A
    
    return True, D, A

def construir_caminho(A: List[int], origem: int, destino: int, D: List[float]) -> List[int]:
    path = []
    atual = destino
    
    while atual is not None:
        path.append(atual)
        atual = A[atual]
    path.reverse()
    
    if path and path[0] != origem and D[destino] != INF:
        path.insert(0, origem)
    
    return path

def formatar_saida(vertice: int, caminho: List[int], distancia: float) -> str:
    saida = f"{vertice}:"
    
    for i, vert in enumerate(caminho):
        saida += str(vert+1)
        if i != len(caminho)-1:
            saida += ","
    
    if distancia == INF:
        saida += "; d=INF"
    elif distancia.is_integer():
        saida += f"; d={int(distancia)}"
    else:
        saida += f"; d={distancia:.1f}"
    
    return saida

def main():
    if len(sys.argv) != 3:
        print(f"Uso: {sys.argv[0]} <arquivo_grafo> <vertice_origem>")
        sys.exit(1)

    try:
        grafo = Grafo()
        grafo.ler(sys.argv[1])
        
        origem = int(sys.argv[2]) - 1  # Convertendo para 0-based
        if origem < 0 or origem >= grafo.qtdVertices():
            print("Vértice de origem inválido!")
            sys.exit(1)
        
        resultado, D, A = BellmanFord(grafo, origem)
        
        if not resultado:
            print("Ciclo negativo detectado no grafo.")
            sys.exit(1)
            
        for v in range(len(D)):
            caminho = construir_caminho(A, origem, v, D)
            print(formatar_saida(v+1, caminho, D[v]))

    except Exception as e:
        print(f"Erro: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()