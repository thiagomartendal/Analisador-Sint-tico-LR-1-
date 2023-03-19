#include "tabela.h"

Tabela::Tabela(Gramatica gmc) {
  this->gmc = gmc;
  this->gmc.extender();
  automato = Automato(this->gmc);
  Tabela::formarAcoesShiftGoto();
  Tabela::formarAcoesReduce();
}

void Tabela::formarAcoesShiftGoto() {
  for (auto &it: automato.getTransicoes()) {
    auto k = it.first;
    auto v = it.second;
    auto buscaChave = tabela.find(k);
    Acao acao;
    if (gmc.terminal(std::get<1>(k))) {
      acao = {TipoAcao::S, v};
    } else {
      acao = {TipoAcao::G, v};
    }
    if(buscaChave == tabela.end()) {
      tabela[k] = acao;
    } else {
      if ((tabela[k].tipo == TipoAcao::S && acao.tipo == TipoAcao::G) || (tabela[k].tipo == TipoAcao::G && acao.tipo == TipoAcao::S))
        throw("Conflito Shift/Goto.");
    }
  }
}

void Tabela::formarAcoesReduce() {
  for (Estado estado: automato.getEstados()) {
    for (Item item: estado.getItens()) {
      if (item.posPonto == item.prod.corpo.size() && item.cabeca != gmc.getInicial()) {
        for (std::string simbolo: item.lookahead) {
          tabela[std::make_tuple(estado.getId(), simbolo)] = {TipoAcao::R, item.prod.numero};
        }
      } else if (item.cabeca == gmc.getInicial() && item.posPonto == 1 && item.lookahead == std::set<std::string>{"$"}) {
        tabela[std::make_tuple(estado.getId(), "$")] = {TipoAcao::ACC, item.prod.numero};
      }
    }
  }
}

bool Tabela::avaliar(std::string sentenca) {
  unsigned int posPonto = 0;
  unsigned int estadoAtual = 0;
  std::string simbolo = "";
  bool reducao = false;
  std::stack<unsigned int> pilha;
  pilha.push(estadoAtual);
  while (true) {
    if (!reducao)
      simbolo = sentenca[posPonto];
    else
      reducao = false;

    auto chave = std::make_tuple(estadoAtual, simbolo);
    auto buscaChave = tabela.find(chave);
    if (buscaChave != tabela.end()) {
      Acao acao = tabela[chave];
      if (acao.tipo == TipoAcao::S || acao.tipo == TipoAcao::G) {
        estadoAtual = acao.destino;
        pilha.push(estadoAtual);
        if (acao.tipo == TipoAcao::S) posPonto += 1;
      } else if (acao.tipo == TipoAcao::R) {
        simbolo = std::get<0>(gmc.producaoPorNumero(acao.destino));
        int desempilhar = std::get<1>(gmc.producaoPorNumero(acao.destino)).corpo.size();
        reducao = true;
        for (int i = 0; i < desempilhar; i++)
          pilha.pop();
        estadoAtual = pilha.top();
      } else if(acao.tipo == TipoAcao::ACC) {
        return true;
      }
    } else break;
  }
  return false;
}

Automato Tabela::getAutomato() {
  return automato;
}

std::map<std::tuple<unsigned int, std::string>, Acao> Tabela::getTabela() {
  return tabela;
}
