# Item = (Cabeca, Corpo, posPonto, lookahead)
# Transicao = [(idEstado1, simbolo)] = idEstado2

class Estado:

    def __init__(self, id): # Como os núcleos indexam os estados no mapa de estados, cada estado pode receber seu id no construtor
        self.__id = id
        self.__itens = []

    def inserirItem(self, item):
        if item not in self.__itens:
            self.__itens.append(item)

    def getId(self):
        return self.__id

    def getItens(self):
        return self.__itens

class Automato:

    def __init__(self, gramatica):
        self.__gmc = gramatica
        self.__idEstado = 0
        self.__estados = {}
        self.__transicoes = {}
        primeiroItem = [tuple([self.__gmc.getInicial(), self.__gmc[self.__gmc.getInicial()][0], 0, tuple('$')])]
        estado = self.__formarEstado(primeiroItem)
        self.__estados[tuple(primeiroItem)] = estado # Cada estado deve ser indexado por seu núcleo de formação no mapa
        self.__construirAutomato(estado)

        for (k,estado) in self.__estados.items():
            print(estado.getId())
            i = 1
            for item in estado.getItens():
                print(
                    i,
                    '(' + item[0],
                    '[' + str(item[1].getCorpo()),
                    str(item[1].getNumero()) + ']',
                    item[2],
                    str(item[3]) + ')'
                )
                i += 1
            print('------------------')
        for (k,v) in self.__transicoes.items():
            print(k, '=', v)
        print('Total de estados:', len(self.__estados))
        print('Total de transições:', len(self.__transicoes))

    def __formarEstado(self, itens):
        estado = Estado(self.__idEstado)
        self.__idEstado += 1
        for item in itens:
            estado.inserirItem(item)
            # O segredo para desenhar o autômato correto era verificar o lookahead dos itens de formação dos estados, e não apenas dos novos itens encontrados
            novoLookahead = self.__formarLookahead(item)
            self.__procurarItens(estado, item, novoLookahead)
        return estado

    def __procurarItens(self, estado, item, lookahead):
        if item[2] < len(item[1].getCorpo()):
            simbolo = item[1].getCorpo()[item[2]]
            if simbolo in self.__gmc.getNaoTerminais():
                for v in self.__gmc[simbolo]:
                    novoItem = tuple([simbolo, v, 0, lookahead])
                    if novoItem not in estado.getItens():
                        estado.inserirItem(novoItem)
                        novoLookahead = self.__formarLookahead(novoItem)
                        self.__procurarItens(estado, novoItem, novoLookahead)

    def __formarLookahead(self, item):
        lookahead = set()
        if len(item[1].getCorpo()) > 1 and item[2]+1 < len(item[1].getCorpo()):
            simbolo = item[1].getCorpo()[item[2]+1]
            if simbolo in self.__gmc.getNaoTerminais():
                lookahead = self.__gmc.first()[simbolo]
            else:
                lookahead.add(simbolo)
        else:
            lookahead = item[3]
        return tuple(lookahead)

    def __formarNucleo(self, estado, simboloTransicao):
        nucleo = []
        for item in estado.getItens():
            if item[2] < len(item[1].getCorpo()):
                simbolo = item[1].getCorpo()[item[2]]
                if simbolo == simboloTransicao:
                    itemNucleo = tuple([item[0], item[1], item[2]+1, item[3]])
                    if itemNucleo not in nucleo:
                        nucleo.append(itemNucleo)
        return sorted(nucleo)

    def __construirAutomato(self, estado):
        for item in estado.getItens():
            if item[2] < len(item[1].getCorpo()):
                simbolo = item[1].getCorpo()[item[2]]
                nucleo = self.__formarNucleo(estado, simbolo)
                if tuple(nucleo) not in self.__estados:
                    novoEstado = self.__formarEstado(nucleo)
                    self.__estados[tuple(nucleo)] = novoEstado
                    self.__transicoes[(estado.getId(), simbolo)] = novoEstado.getId()
                    self.__construirAutomato(novoEstado)
                else:
                    self.__transicoes[(estado.getId(), simbolo)] = self.__estados[tuple(nucleo)].getId()
        # As transições são mais fáceis de se determinar com os id's em cada estado, ao invés de ser chave no mapa de estados
