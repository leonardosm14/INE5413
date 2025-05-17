from grafo import Grafo, Vertice, Aresta
import sys

def DFS_Visit(G: Grafo, v: Vertice, visitados: list, componente: list):
    visitados[v.indice] = True
    componente.append(v)
    
    for u in G.vizinhos(v):
        if not visitados[u.indice]:
            DFS_Visit(G, u, visitados, componente)

def DFS(G: Grafo, ordem=None):
    n = G.qtdVertices()
    visitados = [False] * n
    componentes = []
    
    vertices = G.getVertices()
    if ordem:
        vertices = sorted(vertices, key=lambda v: ordem[v.indice], reverse=True)
    
    for u in vertices:
        if not visitados[u.indice]:
            componente = []
            DFS_Visit(G, u, visitados, componente)
            componentes.append(componente)
    
    return componentes

def CFC(G: Grafo):
    # Primeira DFS para obter ordem de finalização
    F = [0] * G.qtdVertices()
    tempo = [0]
    
    def DFS_Visit_com_tempo(v: Vertice, visitados: list):
        visitados[v.indice] = True
        for u in G.vizinhos(v):
            if not visitados[u.indice]:
                DFS_Visit_com_tempo(u, visitados)
        tempo[0] += 1
        F[v.indice] = tempo[0]
    
    visitados = [False] * G.qtdVertices()
    for u in G.getVertices():
        if not visitados[u.indice]:
            DFS_Visit_com_tempo(u, visitados)

    # Criar grafo transposto com novos objetos de vértices
    vertices_gt = [Vertice(indice=v.indice, rotulo=v.rotulo) for v in G.getVertices()]
    mapa_vertices = {v.indice: v for v in vertices_gt}
    
    arestas_transpostas = [
        Aresta(origem=mapa_vertices[a.destino.indice], destino=mapa_vertices[a.origem.indice])
        for a in G.getArestas()
    ]
    Gt = Grafo(dirigido=True, vertices=vertices_gt, arestas=arestas_transpostas)

    # Segunda DFS com ordem de finalização invertida
    componentes_cfc = DFS(Gt, ordem=F)

    # Converter para lista de strings (1-based)
    return [[str(v.indice + 1) for v in comp] for comp in componentes_cfc]


def main():
    if len(sys.argv) != 2:
        print("Uso: python cfc.py <arquivo_grafo>", file=sys.stderr)
        sys.exit(1)

    try:
        grafo = Grafo(dirigido=True)
        grafo.ler(sys.argv[1])
        
        componentes = CFC(grafo)
        for comp in componentes:
            print(','.join(comp))

    except FileNotFoundError:
        print(f"Erro: Arquivo '{sys.argv[1]}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Erro: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()