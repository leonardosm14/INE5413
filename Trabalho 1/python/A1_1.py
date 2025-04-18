from dataclasses import dataclass, field
import re, sys, argparse
from typing import List

@dataclass
class Vertice:
    rotulo: str
    vizinhos: List['Vertice'] = field(default_factory=list)

@dataclass
class Aresta:
    origem: Vertice
    destino: Vertice
    peso: float

@dataclass
class Grafo:
    vertices: List[Vertice] = field(default_factory=list)
    arestas: List[Aresta] = field(default_factory=list)

    def qtdVertices(self) -> int:
        return len(self.vertices)

    def qtdArestas(self) -> int:
        return len(self.arestas)

    def grau(self, v: Vertice) -> int:
        return len(v.vizinhos)

    def rotulo(self, v: Vertice) -> str:
        return v.rotulo

    def vizinhos(self, v: Vertice) -> List[Vertice]:
        return v.vizinhos

    def haAresta(self, u: Vertice, v: Vertice) -> bool:
        for a in self.arestas:
            if a.origem == u and a.destino == v:
                return True
        return False

    def peso(self, u: Vertice, v: Vertice) -> float:
        for a in self.arestas:
            if a.origem == u and a.destino == v:
                return a.peso
        return -1

    def adicionarVertice(self, rotulo: str) -> Vertice:
        novo_vertice = Vertice(rotulo=rotulo)
        self.vertices.append(novo_vertice)
        return novo_vertice

    def adicionarAresta(self, origem: Vertice, destino: Vertice, peso: float) -> Aresta:
        nova_aresta = Aresta(origem=origem, destino=destino, peso=peso)
        self.arestas.append(nova_aresta)
        origem.vizinhos.append(destino)
        return nova_aresta

    def buscarVertice(self, rotulo: str) -> Vertice | None:
        for v in self.vertices:
            if v.rotulo == rotulo:
                return v
        return None
    
    def ler(self, caminho_arquivo: str):
        with open(caminho_arquivo, 'r') as f:
            linhas = [linha.strip() for linha in f if linha.strip() and not linha.startswith('#')]

        modo = None
        for linha in linhas:
            if linha.lower().startswith("*vertices"):
                modo = "vertices"
                continue
            elif linha.lower().startswith("*edges"):
                modo = "arestas"
                continue

            if modo == "vertices":
                match = re.match(r'^\d+\s+(?:"(.+?)"|(\S+))', linha)
                rotulo = match.group(1) or match.group(2)
                self.adicionarVertice(rotulo=rotulo)
            elif modo == "arestas":
                match = re.match(r'(?:"(.+?)"|(\S+))\s+(?:"(.+?)"|(\S+))\s+([\d.]+)', linha)
                if match:
                    rot1 = match.group(1) or match.group(2)
                    rot2 = match.group(3) or match.group(4)
                    peso = float(match.group(5))
                    self.adicionarAresta(origem=self.buscarVertice(rotulo=rot1), destino=self.buscarVertice(rotulo=rot2), peso=peso)

    def buscarVertice(self, rotulo: str):
        for v in self.vertices:
            if v.rotulo == rotulo:
                return v
        return None

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("arquivo", type=str, help="Arquivo de entrada") 
    args = parser.parse_args()

    grafo = Grafo()
    grafo.ler(args.arquivo)

    print(f"Vértices ({grafo.qtdVertices()}): {[v.rotulo for v in grafo.vertices]}")
    print(f"Arestas ({grafo.qtdArestas()}):")
    for a in grafo.arestas:
        print(f"  {a.origem.rotulo} -> {a.destino.rotulo} (peso={a.peso})")

if __name__ == '__main__':
    main()