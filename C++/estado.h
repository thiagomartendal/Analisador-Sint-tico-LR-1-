#ifndef ESTADO_H
#define ESTADO_H

#include <vector>
#include <set>
#include "gramatica.h"

typedef struct Item {
  std::string cabeca;
  Producao prod;
  unsigned int posPonto;
  std::set<std::string> lookahead;
} Item;

inline bool operator<(const Item& item1, const Item& item2) {
  if (item1.cabeca != item2.cabeca)
    return true;

  if (item1.cabeca.length() < item2.cabeca.length())
    return true;

  if (item1.prod.numero < item2.prod.numero)
    return true;

  if (item1.prod.corpo.size() < item2.prod.corpo.size())
    return true;

  if (item1.prod.corpo != item2.prod.corpo)
    return true;

  if (item1.posPonto < item2.posPonto)
    return true;

  if (item1.lookahead.size() < item2.lookahead.size())
    return true;

  if (item1.lookahead < item2.lookahead)
    return true;

  return item1.lookahead != item2.lookahead;
}

inline bool operator==(const Item& item1, const Item& item2) {
  return (item1.cabeca == item2.cabeca) &&
  (item1.prod.numero == item2.prod.numero) &&
  (item1.prod.corpo == item2.prod.corpo) &&
  (item1.posPonto == item2.posPonto) &&
  (item1.lookahead == item2.lookahead);
}

inline bool operator!=(const Item& item1, const Item& item2) {
  return (item1.cabeca != item2.cabeca) &&
  (item1.prod.numero != item2.prod.numero) &&
  (item1.prod.corpo != item2.prod.corpo) &&
  (item1.posPonto != item2.posPonto) &&
  (item1.lookahead != item2.lookahead);
}

class Estado {
private:
  unsigned int id;
  std::vector<Item> itens;

public:
  Estado() = default;
  Estado(unsigned int id);
  bool operator==(Estado estado);
  bool existeItem(Item item);
  void inserirItem(Item item);
  unsigned int getId();
  std::vector<Item> getItens();
};

#endif
