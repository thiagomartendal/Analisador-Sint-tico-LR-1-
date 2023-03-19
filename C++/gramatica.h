#ifndef GRAMATICA_H
#define GRAMATICA_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

typedef struct Producao {
  std::vector<std::string> corpo;
  unsigned int numero;
} Producao;

inline bool operator==(const Producao& prod1, const Producao& prod2) {
  return (prod1.corpo == prod2.corpo && prod1.numero == prod2.numero);
}

// EPS = Épsilon (&)
// EOF = Fim de Arquivo ($)

class Gramatica {
private:
  std::map<std::string, std::vector<Producao>> gmc;
  std::map<std::string, std::set<std::string>> conjuntoFirst;
  std::set<std::string> terminais, naoTerminais;
  std::string inicial{""}, eps{"EPS"}, eof{"EOF"};
  bool extendida{false};
  unsigned int numProducao{0};
  void refatorarNumeroProducoes();
  void iniciarFirst();
  void primeiraBuscaPorEpsilon();
  void segundaBuscaPorEpsilon();
  void buscaTerminaisIniciais();
  void buscaProximosTerminais();
  void atualizarFirst();
  bool firstComNaoTerminal();

public:
  Gramatica(std::string eps = "EPS", std::string eof = "EOF");
  void operator()(std::string chave, std::vector<std::vector<std::string>> valor);
  std::vector<Producao> operator[](std::string chave);
  void setTerminais(std::set<std::string> terminais);
  void setNaoTerminais(std::set<std::string> naoTerminais);
  std::set<std::string> getTerminais();
  std::set<std::string> getNaoTerminais();
  std::tuple<std::string, Producao> producaoPorNumero(unsigned int numero);
  std::string getInicial();
  bool terminal(std::string simbolo);
  bool naoTerminal(std::string simbolo);
  void extender();
  void exibir();
  void exibirPorNumero();
  std::map<std::string, std::set<std::string>> first();
};

#endif
