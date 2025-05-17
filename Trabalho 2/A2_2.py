from grafo import Grafo, Vertice
import sys

def DFS_Visit_OT(G: Grafo, v: Vertice, visitados: list, ordem: list):
    visitados[v.indice] = True
    
    for u in G.vizinhos(v=v):
        if not visitados[u.indice]:
            DFS_Visit_OT(G=G, v=u, visitados=visitados, ordem=ordem)
    
    ordem.append(v.rotulo)  # Armazena o rótulo do vértice

def DFS_OT(G: Grafo):
    n = G.qtdVertices()
    visitados = [False] * n
    ordem = list()

    for u in G.getVertices():
        if not visitados[u.indice]:
            DFS_Visit_OT(G=G, v=u, visitados=visitados, ordem=ordem)
    
    return ordem[::-1]  # Inverte a ordem para obter a topológica

def main():
    if len(sys.argv) != 2:
        print("Uso: python ordenacao_topologica.py <arquivo_grafo>", file=sys.stderr)
        sys.exit(1)

    try:
        grafo = Grafo(dirigido=True)
        grafo.ler(sys.argv[1])
        
        ordem_topologica = DFS_OT(G=grafo)
        print(', '.join(ordem_topologica))

    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()