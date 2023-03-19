#include <iostream>
#include <vector>
#include "gramatica.h"
#include "automato.h"
#include "tabela.h"

int main() {
  Gramatica gmc;

  // gmc("S", {{"A", "A"}});
  // gmc("A", {{"a", "A"}, {"b"}});
  // gmc.setTerminais({"a", "b"});
  // gmc.setNaoTerminais({"S", "A"});

  // gmc("S", {{"(", "L", ")"}, {"x"}});
  // gmc("L", {{"S"}, {"L", ";", "S"}});
  // gmc.setTerminais({"(", ")", ";", "x"});
  // gmc.setNaoTerminais({"S", "L"});

  // gmc("E", {{"E", "+", "T"}, {"T"}});
  // gmc("T", {{"T", "*", "F"}, {"F"}});
  // gmc("F", {{"(", "E", ")"}, {"id"}});
  // gmc.setTerminais({"+", "*", "(", ")", "id"});
  // gmc.setNaoTerminais({"E", "T", "F"});

  gmc("E", {{"E", "+", "T"}, {"T"}});
  gmc("T", {{"T", "*", "F"}, {"F"}});
  gmc("F", {{"(", "E", ")"}, {"i"}});
  gmc.setTerminais({"+", "*", "(", ")", "i"});
  gmc.setNaoTerminais({"E", "T", "F"});

  // Gramatica gmc("&");
  // gmc("P", {{"K", "V", "C"}});
  // gmc("K", {{"c", "K"}, {"&"}});
  // gmc("V", {{"v", "V"}, {"F"}});
  // gmc("F", {{"f", "P", ";", "F"}, {"&"}});
  // gmc("C", {{"b", "V", "C", "e"}, {"com", ";", "C"}, {"&"}});
  // gmc.setTerminais({"c", "v", "f", ";", "b", "e", "com"});
  // gmc.setNaoTerminais({"P", "K", "V", "F", "C"});

  // gmc("S", {{"L", "=", "R"}, {"R"}});
  // gmc("L", {{"*", "R"}, {"id"}});
  // gmc("R", {{"L"}});
  // gmc.setTerminais({"=", "*", "id"});
  // gmc.setNaoTerminais({"S", "L", "R"});

  // gmc("S", {{"E"}});
  // gmc("E", {{"T"}, {"E", "+", "T"}});
  // gmc("T", {{"int"}, {"(", "E", ")"}});
  // gmc.setTerminais({"+", "int", "(", ")"});
  // gmc.setNaoTerminais({"S", "E", "T"});

  // gmc.extender();

  // Automato aut = Automato(gmc);

  Tabela tabela = Tabela(gmc);

  for (Estado e: tabela.getAutomato().getEstados()) {
    std::cout << e.getId() << '\n';
    int i = 1;
    for (Item item: e.getItens()) {
      std::cout << i << " (" << item.cabeca << " [";
      for (std::string str: item.prod.corpo)
        std::cout << str << ' ';
      std::cout << item.prod.numero << "] " << item.posPonto<< " {";
      for (std::string str: item.lookahead)
        std::cout << str << ' ';
      std::cout << "})\n";
      i++;
    }
    std::cout << "------------------\n";
  }

  for (auto& it: tabela.getAutomato().getTransicoes())
    std::cout << '(' << std::get<0>(it.first) << ',' << std::get<1>(it.first) << ") = " << it.second << '\n';

  std::cout << "Total de estados: " << tabela.getAutomato().getEstados().size() << '\n';
  std::cout << "Total de transições: " << tabela.getAutomato().getTransicoes().size() << '\n';

  for (auto &it: tabela.getTabela()) {
    auto k = it.first;
    auto v = it.second;
    std::cout << '[' << std::get<0>(k) << "][" << std::get<1>(k) << "] = (" << v.tipo << "," << v.destino << ")\n";
  }

  std::string sentenca = "(i*i)+(i*i)$";
  bool avaliacao = tabela.avaliar(sentenca);
  std::cout << sentenca << " " << (avaliacao ? "Correto" : "Incorreto") << std::endl;

  return 0;
}
