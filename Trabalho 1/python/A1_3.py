import argparse
from typing import List, Tuple, Optional, Dict
from collections import defaultdict
from A1_1 import Grafo, Vertice, Aresta

def Hierholzer(G: Grafo) -> Tuple[bool, Optional[List[str]]]:
    if G.qtdArestas() == 0:
        return (True, [])

    # Contador de arestas não visitadas (usando rótulos)
    C = defaultdict(int)
    for aresta in G.arestas:
        u = aresta.origem.rotulo
        v = aresta.destino.rotulo
        key = tuple(sorted((u, v)))
        C[key] += 1

    # Encontrar um vértice inicial com arestas não visitadas
    v_inicial = None
    for vertice in G.vertices:
        for vizinho in vertice.vizinhos:
            key = tuple(sorted((vertice.rotulo, vizinho.rotulo)))
            if C[key] > 0:
                v_inicial = vertice
                break
        if v_inicial is not None:
            break

    if v_inicial is None:
        return (False, None)

    r, Ciclo = buscar_subciclo(G, v_inicial, C)
    if not r:
        return (False, None)

    if any(count > 0 for count in C.values()):
        return (False, None)

    return (True, Ciclo)

def buscar_subciclo(G: Grafo, v: Vertice, C: Dict[Tuple[str, str], int]) -> Tuple[bool, Optional[List[str]]]:
    Ciclo = [v.rotulo]
    t = v

    while True:
        aresta_encontrada = False
        for u in v.vizinhos:
            key = tuple(sorted((v.rotulo, u.rotulo)))
            if C[key] > 0:
                C[key] -= 1
                v = u
                Ciclo.append(v.rotulo)
                aresta_encontrada = True
                break

        if not aresta_encontrada:
            return (False, None)

        if v == t:
            break

    # Inserir subciclos
    i = 0
    while i < len(Ciclo):
        x_rotulo = Ciclo[i]
        x = next((v for v in G.vertices if v.rotulo == x_rotulo), None)

        if x is None:
            return (False, None)

        tem_arestas = False
        for u in x.vizinhos:
            key = tuple(sorted((x.rotulo, u.rotulo)))
            if C[key] > 0:
                tem_arestas = True
                break

        if tem_arestas:
            r, subciclo = buscar_subciclo(G, x, C)
            if not r:
                return (False, None)
            subciclo_sem_repeticao = subciclo[:-1]
            Ciclo = Ciclo[:i] + subciclo_sem_repeticao + Ciclo[i:]
            i = 0  # Reinicia a verificação
        else:
            i += 1

    return (True, Ciclo)

def main():
    parser = argparse.ArgumentParser(description="Implementação do algoritmo de Hierholzer para ciclo euleriano")
    parser.add_argument("arquivo", help="Arquivo de entrada no formato especificado")
    args = parser.parse_args()

    grafo = Grafo()
    grafo.ler(args.arquivo)

        
    r, ciclo = Hierholzer(grafo)
    if not r:
        print(0)
    else:
        rotulo_para_indice = {v.rotulo: str(v.indice) for v in grafo.vertices}
        caminho_indices = [rotulo_para_indice[r] for r in ciclo]
        print("1\n" + ",".join(caminho_indices))
    

if __name__ == '__main__':
    main()