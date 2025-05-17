from dataclasses import dataclass, field
from typing import List, Dict

@dataclass
class Vertice:
    indice: int
    rotulo: str
    vizinhos: List['Vertice'] = field(default_factory=list)

@dataclass
class Aresta:
    origem: Vertice
    destino: Vertice
    peso: float = None

@dataclass
class Grafo:
    dirigido: bool = False
    vertices: List[Vertice] = field(default_factory=list)
    arestas: List[Aresta] = field(default_factory=list)
    indice_para_vertice: Dict[int, Vertice] = field(default_factory=dict)

    def qtdVertices(self) -> int:
        return len(self.vertices)

    def qtdArestas(self) -> int:
        return len(self.arestas)

    def getVertices(self) -> List[Vertice]:
        return self.vertices
    
    def getArestas(self) -> List[Aresta]:
        return self.arestas

    def grau(self, v: Vertice) -> int:
        return len(v.vizinhos)

    def rotulo(self, v: Vertice) -> str:
        return v.rotulo

    def vizinhos(self, v: Vertice) -> List[Vertice]:
        return v.vizinhos

    def haAresta(self, u: Vertice, v: Vertice) -> bool:
        for a in self.arestas:
            if (a.origem == u and a.destino == v) or (a.origem == v and a.destino == u):
                return True
        return False

    def peso(self, u: Vertice, v: Vertice) -> float:
        for a in self.arestas:
            if (a.origem == u and a.destino == v) or (a.origem == v and a.destino == u):
                return a.peso
        return float('inf')

    def adicionarVertice(self, indice: int, rotulo: str) -> Vertice:
        novo_vertice = Vertice(indice=indice, rotulo=rotulo)
        self.vertices.append(novo_vertice)
        self.indice_para_vertice[indice] = novo_vertice
        return novo_vertice

    def adicionarAresta(self, origem: Vertice, destino: Vertice, peso: float = 1) -> Aresta:
        nova_aresta = Aresta(origem=origem, destino=destino, peso=peso)
        self.arestas.append(nova_aresta)
        origem.vizinhos.append(destino)
        if not self.dirigido:
            destino.vizinhos.append(origem)
        return nova_aresta

    def buscarVerticePorIndice(self, indice: int) -> Vertice | None:
        return self.indice_para_vertice.get(indice)
    
    def ler(self, caminho_arquivo: str):
        """Lê arquivo no formato .net, procurando por *arcs ou *edges conforme o grafo seja dirigido"""
        with open(caminho_arquivo, 'r') as f:
            linhas = [linha.strip() for linha in f if linha.strip() and not linha.startswith('#')]
        
        if not linhas:
            raise ValueError("Arquivo vazio ou contém apenas comentários")
        
        # Processa vértices
        vertices_linha = linhas[0]
        if not vertices_linha.lower().startswith("*vertices"):
            raise ValueError("Formato inválido: arquivo deve começar com *vertices")
        
        vertices_data = linhas[1:]  # Pega todas as linhas após *vertices
        
        # Procura por *arcs ou *edges conforme o grafo seja dirigido ou não
        if self.dirigido:
            marker = "*arcs"
        else:
            marker = "*edges"
        
        edges_index = next((i for i, linha in enumerate(vertices_data) 
                        if linha.lower().startswith(marker)), None)
        
        if edges_index is None:
            vertices_lines = vertices_data
        else:
            vertices_lines = vertices_data[:edges_index]
        
        for linha in vertices_lines:
            try:
                partes = linha.split(maxsplit=1)
                indice = int(partes[0])
                rotulo = partes[1].strip('"') if len(partes) > 1 else ""
                self.adicionarVertice(indice-1, rotulo)
            except (IndexError, ValueError) as e:
                raise ValueError(f"Erro ao processar vértice: '{linha}': {e}")
        
        # Processa arestas/arcos se existirem
        if edges_index is not None:
            for linha in vertices_data[edges_index+1:]:
                try:
                    partes = linha.split()
                    origem_idx = int(partes[0])
                    destino_idx = int(partes[1])
                    peso = float(partes[2]) if len(partes) > 2 else 1.0
                    
                    v_origem = self.buscarVerticePorIndice(origem_idx-1)
                    v_destino = self.buscarVerticePorIndice(destino_idx-1)
                    
                    if v_origem is None:
                        raise ValueError(f"Vértice de origem {origem_idx} não encontrado")
                    if v_destino is None:
                        raise ValueError(f"Vértice de destino {destino_idx} não encontrado")
                    
                    self.adicionarAresta(v_origem, v_destino, peso)
                except (IndexError, ValueError) as e:
                    raise ValueError(f"Erro ao processar aresta: '{linha}': {e}")