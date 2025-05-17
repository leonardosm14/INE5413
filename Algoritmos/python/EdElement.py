from dataclasses import dataclass, field


@dataclass
class EdElement:
    pai: "EdElement" = field(init=False)
    rank: int = 0

    def __post_init__(self):
        self.pai = self

    def cdLigar(self, x: "EdElement", y: "EdElement"):
        if (x.rank > y.rank):
            y.pai = x
        else:
            x.pai = y
            if (x.rank == y.rank):
                y.rank += 1
    
    def cdEncontra(self, x: "EdElement"):
        if (x.pai != x):
            x.pai = self.cdEncontra(x=x.pai)
        return x.pai
    
    def cdUniao(self, x: "EdElement", y: "EdElement"):
        self.cdLigar(x=self.cdEncontra(x=x),
                     y=self.cdEncontra(x=y))