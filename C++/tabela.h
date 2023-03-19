#ifndef TABELA_H
#define TABELA_H

#include <stack>
#include <map>
// #include "gramatica.h"
#include "automato.h"

typedef enum TipoAcao {
  S, // Shift
  G, // Goto
  R, // Reduce
  ACC // Aceita
} TipoAcao;

typedef struct Acao {
  TipoAcao tipo;
  unsigned int destino;
} Acao;

class Tabela {
private:
  std::map<std::tuple<unsigned int, std::string>, Acao> tabela;
  Gramatica gmc;
  Automato automato;
  void formarAcoesShiftGoto();
  void formarAcoesReduce();

public:
  Tabela(Gramatica gmc);
  bool avaliar(std::string sentenca);
  Automato getAutomato();
  std::map<std::tuple<unsigned int, std::string>, Acao> getTabela();
};

#endif
