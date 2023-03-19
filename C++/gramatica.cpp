#include "gramatica.h"

Gramatica::Gramatica(std::string eps, std::string eof) {
  this->eps = eps;
  this->eof = eof;
  this->terminais.insert(this->eps);
  this->terminais.insert(this->eof);
}

void Gramatica::operator()(std::string chave, std::vector<std::vector<std::string>> valor) {
  if (inicial == "") {
    inicial = chave;
  }
  std::vector<Producao> novoValor;
  for (std::vector<std::string> x: valor) {
    novoValor.push_back(Producao{x, numProducao});
    numProducao++;
  }
  gmc[chave] = novoValor;
}

std::vector<Producao> Gramatica::operator[](std::string chave) {
  return gmc[chave];
}

void Gramatica::setTerminais(std::set<std::string> terminais) {
  this->terminais.insert(terminais.begin(), terminais.end());
}

void Gramatica::setNaoTerminais(std::set<std::string> naoTerminais) {
  this->naoTerminais = naoTerminais;
}

std::string Gramatica::getInicial() {
  return inicial;
}

std::set<std::string> Gramatica::getTerminais() {
  return terminais;
}

std::set<std::string> Gramatica::getNaoTerminais() {
  return naoTerminais;
}

std::tuple<std::string, Producao> Gramatica::producaoPorNumero(unsigned int numero) {
  std::string cabeca = "";
  Producao producao;
  for (auto &it: gmc)
    for (Producao prod: it.second)
      if (prod.numero == numero) {
        cabeca = it.first;
        producao = prod;
        break;
      }
  return std::make_tuple(cabeca, producao);
}

bool Gramatica::terminal(std::string simbolo) {
  return (std::find(terminais.begin(), terminais.end(), simbolo) != terminais.end());
}

bool Gramatica::naoTerminal(std::string simbolo) {
  return (std::find(naoTerminais.begin(), naoTerminais.end(), simbolo) != naoTerminais.end());
}

void Gramatica::extender() {
  if (extendida == false) {
    std::string novoInicial = inicial+"'";
    gmc[novoInicial] = {Producao{{inicial}, 0}};
    naoTerminais.insert(novoInicial);
    inicial = novoInicial;
    extendida = true;
    Gramatica::refatorarNumeroProducoes();
  }
}

void Gramatica::refatorarNumeroProducoes() {
  std::map<std::string, std::vector<Producao>> tmpGmc;
  tmpGmc[inicial] = {Producao{gmc[inicial][0].corpo, 0}};
  numProducao = 1;
  for (auto& it: gmc) {
    auto k = it.first;
    auto v = it.second;
    if (k != inicial) {
      tmpGmc[k] = {};
      for (Producao producao: v) {
        tmpGmc[k].push_back(Producao{producao.corpo, numProducao});
        numProducao++;
      }
    }
  }
  gmc = tmpGmc;
}

void Gramatica::exibir() {
  for (auto& it: gmc) {
    auto k = it.first;
    auto v = it.second;
    std::string p = "";
    int i = 0;
    for (Producao producao: v) {
      for (std::string str: producao.corpo) {
        p += str;
      }
      if (i < v.size()-1) {
        p += '|';
      }
      i++;
    }
    std::cout << k << "->" << p << std::endl;
  }
}

void Gramatica::exibirPorNumero() {
  for (auto& it: gmc) {
    auto k = it.first;
    auto v = it.second;
    for (Producao producao: v) {
      std::string p = "";
      for (std::string str: producao.corpo) {
        p += str;
      }
      std:: cout << producao.numero << " " << k << "->" << p << std::endl;
    }
  }
}

std::map<std::string, std::set<std::string>> Gramatica::first() {
  Gramatica::iniciarFirst();
  Gramatica::primeiraBuscaPorEpsilon();
  Gramatica::segundaBuscaPorEpsilon();
  Gramatica::buscaTerminaisIniciais();
  Gramatica::buscaProximosTerminais();
  Gramatica::atualizarFirst();
  return conjuntoFirst;
}

void Gramatica::iniciarFirst() {
  for (std::string k: terminais) {
    if (k != eps)
      conjuntoFirst[k].insert(k);
  }
}

void Gramatica::primeiraBuscaPorEpsilon() {
  for (auto& it: gmc) {
    auto k = it.first;
    auto v = it.second;
    for (Producao x: v) {
      if (x.corpo == std::vector<std::string>{eps}) {
        conjuntoFirst[k].insert(eps);
      }
    }
  }
}

void Gramatica::segundaBuscaPorEpsilon() {
  for (auto it = gmc.rbegin(); it != gmc.rend(); it++) {
    auto k = it->first;
    auto v = it->second;
    for (Producao x: v) {
      for (std::string y: x.corpo) {
        if ((naoTerminais.find(y) != naoTerminais.end()) && (conjuntoFirst[y].find(eps) != conjuntoFirst[y].end())){
          std::vector<std::string>::iterator elemento = std::find(x.corpo.begin(), x.corpo.end(), y);
          int indice = std::distance(x.corpo.begin(), elemento);
          if (indice == x.corpo.size()-1) {
            conjuntoFirst[k].insert(eps);
          }
        } else {
          break;
        }
      }
    }
  }
}

void Gramatica::buscaTerminaisIniciais() {
  for (auto& it: gmc) {
    auto k = it.first;
    auto v = it.second;
    for (Producao x: v) {
      if (terminais.find(x.corpo[0]) != terminais.end()) {
        conjuntoFirst[k].insert(x.corpo[0]);
      }
    }
  }
}

void Gramatica::buscaProximosTerminais() {
  for (auto it = gmc.rbegin(); it != gmc.rend(); it++) {
    auto k = it->first;
    auto v = it->second;
    for (Producao x: v) {
      bool anteriorComEpsilon = false;
      for (std::string y: x.corpo) {
        if (naoTerminais.find(y) != naoTerminais.end() && y != k) {
          conjuntoFirst[k].insert(y);
          if (conjuntoFirst[y].find(eps) != conjuntoFirst[y].end()) {
            anteriorComEpsilon = true;
            std::vector<std::string>::iterator elemento = std::find(x.corpo.begin(), x.corpo.end(), y);
            int indice = std::distance(x.corpo.begin(), elemento);
            if (indice == x.corpo.size()-1)
              conjuntoFirst[k].insert(eps);
          } else {
            anteriorComEpsilon = false;
            break;
          }
        } else {
          if (anteriorComEpsilon) {
            if (y != k)
              conjuntoFirst[k].insert(y);
            if (naoTerminais.find(y) == naoTerminais.end() || conjuntoFirst[y].find(eps) == conjuntoFirst[y].end())
                break;
          } else {
            std::vector<std::string>::iterator elemento = std::find(x.corpo.begin(), x.corpo.end(), y);
            int indice = std::distance(x.corpo.begin(), elemento);
            if (indice == 0 && naoTerminais.find(y) != naoTerminais.end() && conjuntoFirst[y].find(eps) != conjuntoFirst[y].end())
              anteriorComEpsilon = true;
            else
              break;
          }
        }
      }
    }
  }
}

void Gramatica::atualizarFirst(){
  while (Gramatica::firstComNaoTerminal()) {
    for (auto& it: conjuntoFirst) {
      auto k = it.first;
      auto v = it.second;
      for (std::string x: v) {
        if (naoTerminais.find(x) != naoTerminais.end()) {
          conjuntoFirst[k].erase(x);
          for (std::string y: conjuntoFirst[x]) {
            if (y != eps)
              conjuntoFirst[k].insert(y);
          }
        }
      }
    }
  }
}

bool Gramatica::firstComNaoTerminal() {
  for (auto& it: conjuntoFirst) {
    auto k = it.first;
    auto v = it.second;
    for (std::string x: v) {
      if (naoTerminais.find(x) != naoTerminais.end())
        return true;
    }
  }
  return false;
}
