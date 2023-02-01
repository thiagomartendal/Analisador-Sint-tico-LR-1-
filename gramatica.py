class Producao:

    def __init__(self, corpo, numero):
        self.__corpo = corpo
        self.__numero = numero

    def setNumero(self, numero):
        self.__numero = numero

    def getCorpo(self):
        return self.__corpo

    def getNumero(self):
        return self.__numero

class Gramatica:

    def __init__(self):
        self.__gmc = {}
        self.__inicial = ''
        self.__extendida = False
        self.__numProducao = 0

    def __setitem__(self, chave, valor):
        if self.__inicial == '':
            self.__inicial = chave
        novoValor = []
        for x in valor:
            novoValor.append(Producao(tuple(x), self.__numProducao))
            self.__numProducao += 1
        self.__gmc[chave] = novoValor

    def __getitem__(self, chave):
        return self.__gmc[chave]

    def setTerminais(self, terminais):
        self.__terminais = terminais

    def setNaoTerminais(self, naoTerminais):
        self.__naoTerminais = naoTerminais

    def getTerminais(self):
        return self.__terminais

    def getNaoTerminais(self):
        return self.__naoTerminais

    def getInicial(self):
        return self.__inicial

    def get(self):
        return self.__gmc

    def extender(self):
        if self.__extendida is False:
            novoInicial = self.__inicial+'\''
            self.__gmc[novoInicial] = [Producao(tuple(self.__inicial), 0)]
            self.__naoTerminais.append(novoInicial)
            self.__inicial = novoInicial
            self.__extendida = True
            self.__refatorarNumeroProducoes()

    def __refatorarNumeroProducoes(self):
        tmpGmc = {}
        tmpGmc[self.__inicial] = [Producao(self.__gmc[self.__inicial][0].getCorpo(), 0)]
        self.__numProducao = 1
        for (k,v) in self.__gmc.items():
            if k != self.__inicial:
                tmpGmc[k] = []
                for producao in v:
                    tmpGmc[k].append(Producao(producao.getCorpo(), self.__numProducao))
                    self.__numProducao += 1
        self.__gmc = tmpGmc

    def exibir(self):
        for (k,v) in self.__gmc.items():
            p = ''
            i = 0
            for producao in v:
                p += ''.join(producao.getCorpo())
                if i < len(v)-1:
                    p += '|'
                i += 1
            print(k, '->', p)

    def exibirPorNumero(self):
        for (k,v) in self.__gmc.items():
            for producao in v:
                p = ''.join(producao.getCorpo())
                print(producao.getNumero(), k, '->', p)

    def first(self):
        self.__iniciarFirst()
        self.__primeiraBuscaPorEpsilon()
        self.__segundaBuscaPorEpsilon()
        self.__buscaTerminaisIniciais()
        self.__buscaProximosTerminais()
        self.__atualizarFirst()
        for k in self.__first:
            self.__first[k] = sorted(self.__first[k])
        return self.__first

    def __iniciarFirst(self):
        self.__first = {}
        for k in self.__naoTerminais:
            self.__first[k] = set()
        for k in self.__terminais:
            if k != '&':
                self.__first[k] = set()
                self.__first[k].add(k)

    def __primeiraBuscaPorEpsilon(self):
        for (k,v) in self.__gmc.items():
            for x in v:
                if x.getCorpo() == tuple('&'):
                    self.__first[k].add('&')

    def __segundaBuscaPorEpsilon(self):
        for (k,v) in reversed(self.__gmc.items()):
            for x in v:
                for y in x.getCorpo():
                    if y in self.__naoTerminais and '&' in self.__first[y]:
                        if x.getCorpo().index(y) == len(x.getCorpo())-1:
                            self.__first[k].add('&')
                    else:
                        break

    def __buscaTerminaisIniciais(self):
        for (k,v) in self.__gmc.items():
            for x in v:
                if x.getCorpo()[0] in self.__terminais:
                    self.__first[k].add(x.getCorpo()[0])

    def __buscaProximosTerminais(self):
        for (k,v) in reversed(self.__gmc.items()):
            for x in v:
                anteriorComEpsilon = False
                for y in x.getCorpo():
                    if y in self.__naoTerminais and y != k:
                        self.__first[k].add(y)
                        if '&' in self.__first[y]:
                            anteriorComEpsilon = True
                            if x.getCorpo().index(y) == len(x.getCorpo())-1:
                                self.__first[k].add('&')
                        else:
                            anteriorComEpsilon = False
                            break
                    else:
                        if anteriorComEpsilon:
                            if y != k:
                                self.__first[k].add(y)
                            if y not in self.__naoTerminais or '&' not in self.__first[y]:
                                break
                        else:
                            if x.getCorpo().index(y) == 0 and y in self.__naoTerminais and '&' in self.__first[y]:
                                anteriorComEpsilon = True
                            else:
                                break

    def __atualizarFirst(self):
        while self.__firstComNaoTerminal():
            for (k,v) in self.__first.items():
                for x in v.copy():
                    if x in self.__naoTerminais:
                        self.__first[k].remove(x)
                        for y in self.__first[x]:
                            if y != '&':
                                self.__first[k].add(y)

    def __firstComNaoTerminal(self):
        for (k,v) in self.__first.items():
            for x in v:
                if x in self.__naoTerminais:
                    return True
        return False
