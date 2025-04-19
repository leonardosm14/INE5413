from typing import Dict, List
from A1_1 import Grafo, Vertice, Aresta
import sys

INF = "INF"

def BellmanFord(G: Grafo, s: Vertice):
    vertices = G.getVertices()
    arestas = G.getArestas()
    
    # Inicialização com INF
    D = {v.indice: INF for v in vertices}
    A = {v.indice: None for v in vertices}
    D[s.indice] = 0  # Distância para a própria origem é zero

    # Relaxação das arestas
    for _ in range(len(vertices) - 1):
        for aresta in arestas:
            u = aresta.origem.indice
            v = aresta.destino.indice
            
            if D[u] == INF:
                continue
                
            if D[v] == INF or D[u] + aresta.peso < D[v]:
                D[v] = D[u] + aresta.peso
                A[v] = u

    # Verificação de ciclos negativos
    for aresta in arestas:
        u = aresta.origem.indice
        v = aresta.destino.indice
        
        if D[u] == INF:
            continue
            
        if D[v] == INF or D[u] + aresta.peso < D[v]:
            return (False, None, None)
    
    return (True, D, A)

def construir_caminho(v_indice: int, A: Dict[int, int], s_indice: int) -> str:
    if A[v_indice] is None and v_indice != s_indice:
        return f"{s_indice},{v_indice}"  # Mostra origem->destino para inalcançáveis
    
    caminho = []
    atual = v_indice
    
    while atual is not None and atual != s_indice:
        caminho.append(str(atual))
        atual = A[atual]
    
    caminho.append(str(s_indice))
    caminho.reverse()
    
    return ','.join(caminho)

def main():
    if len(sys.argv) != 3:
        print(f"Uso: {sys.argv[0]} <arquivo_grafo> <vertice_origem>")
        sys.exit(1)

    try:
        grafo = Grafo()
        grafo.ler(sys.argv[1])
        
        origem_indice = int(sys.argv[2])
        s = grafo.buscarVerticePorIndice(origem_indice)
        
        if s is None:
            print(f"Erro: Vértice de origem {origem_indice} não existe no grafo")
            sys.exit(1)

        resultado, D, A = BellmanFord(grafo, s)
        
        if not resultado:
            print("O grafo contém ciclo negativo alcançável da origem!")
            sys.exit(0)

        # Gerar saída ordenada
        for v in sorted(grafo.getVertices(), key=lambda x: x.indice):
            distancia = D[v.indice]
            caminho = construir_caminho(v.indice, A, s.indice)
            
            if distancia == INF:
                print(f"{v.indice}: {caminho}; d=INF")
            elif isinstance(distancia, float) and distancia.is_integer():
                print(f"{v.indice}: {caminho}; d={int(distancia)}")
            else:
                print(f"{v.indice}: {caminho}; d={distancia}")

    except Exception as e:
        print(f"Erro: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()