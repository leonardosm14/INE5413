from dataclasses import dataclass, field
from typing import List, Dict, Union

@dataclass
class Vertice:
    indice: int
    rotulo: str
    vizinhos: List['Vertice'] = field(default_factory=list)

    def __hash__(self):
        return hash(self.indice)

    def __eq__(self, other):
        if isinstance(other, Vertice):
            return self.indice == other.indice
        return False

@dataclass
class Aresta:
    origem: Vertice
    destino: Vertice
    peso: float = None

@dataclass
class Grafo:
    dirigido: bool = False
    bipartido: bool = False
    vertices: List[Vertice] = field(default_factory=list)
    arestas: List[Aresta] = field(default_factory=list)
    indice_para_vertice: Dict[int, Vertice] = field(default_factory=dict)
    X: List[Vertice] = field(default_factory=list)  # Parte X da bipartição
    Y: List[Vertice] = field(default_factory=list)  # Parte Y da bipartição

    def getVertice(self, label: str) -> Union[Vertice, None]:
        for v in self.vertices:
            if v.rotulo == label:
                return v
        return None

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
            if self.dirigido:
                if (a.origem == u and a.destino == v):
                    return a.peso
            else:
                if (a.origem == u and a.destino == v) or (a.origem == v and a.destino == u):
                    return a.peso
                
        return float('inf')

    def adicionarVertice(self, indice: int, rotulo: str) -> Vertice:
        novo_vertice = Vertice(indice=indice, rotulo=rotulo)
        self.vertices.append(novo_vertice)
        self.indice_para_vertice[indice] = novo_vertice
        return novo_vertice

    def adicionarAresta(self, origem: Vertice, destino: Vertice, peso: float) -> Aresta:
        nova_aresta = Aresta(origem=origem, destino=destino, peso=peso)
        self.arestas.append(nova_aresta)
        origem.vizinhos.append(destino)
        if not self.dirigido:
            destino.vizinhos.append(origem)
        return nova_aresta

    def buscarVerticePorIndice(self, indice: int) -> Vertice | None:
        return self.indice_para_vertice.get(indice)
    
    def ler(self, caminho_arquivo: str):
        """Lê arquivo no formato .net e separa automaticamente X e Y."""
        with open(caminho_arquivo, 'r') as f:
            linhas = [linha.strip() for linha in f if linha.strip() and not linha.startswith('#')]
        
        if not linhas:
            raise ValueError("Arquivo vazio ou contém apenas comentários")
        
        # Processa vértices
        if not linhas[0].lower().startswith("*vertices"):
            raise ValueError("Formato inválido: arquivo deve começar com *vertices")
        
        vertices_data = linhas[1:]
        
        # Encontra a linha *edges ou *arcs
        marker = "*arcs" if self.dirigido else "*edges"
        edges_index = next((i for i, linha in enumerate(vertices_data) 
                        if linha.lower().startswith(marker)), None)
        
        # Processa vértices (todos ficam em self.vertices)
        vertices_lines = vertices_data[:edges_index] if edges_index is not None else vertices_data
        for linha in vertices_lines:
            partes = linha.split(maxsplit=1)
            indice = int(partes[0])
            rotulo = str(partes[1].strip('"')) if len(partes) > 1 else ""
            self.adicionarVertice(indice - 1, rotulo)  # Armazena em self.vertices
        
        # Define X e Y (bipartição automática)
        n = self.qtdVertices()
        split_idx = n // 2  # ⌊n/2⌋
        self.X = [v for v in self.vertices if v.indice < split_idx]
        self.Y = [v for v in self.vertices if v.indice >= split_idx]
        
        # Processa arestas (se existirem)
        if edges_index is not None:
            for linha in vertices_data[edges_index + 1:]:
                partes = linha.split()
                origem_idx = int(partes[0]) - 1
                destino_idx = int(partes[1]) - 1
                peso = float(partes[2]) if len(partes) > 2 else 1.0
                
                origem = self.buscarVerticePorIndice(origem_idx)
                destino = self.buscarVerticePorIndice(destino_idx)
                
                if not origem or not destino:
                    raise ValueError(f"Aresta inválida: vértice não encontrado ({linha})")
                
                self.adicionarAresta(origem, destino, peso)
        
    def getX(self) -> List[Vertice]:
        """Retorna a partição X (vértices 1 a ⌊n/2⌋)."""
        return self.X

    def getY(self) -> List[Vertice]:
        """Retorna a partição Y (vértices ⌊n/2⌋+1 a n)."""
        return self.Y