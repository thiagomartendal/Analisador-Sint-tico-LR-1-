#include "estado.h"

Estado::Estado(unsigned int id) {
  this->id = id;
}

bool Estado::operator==(Estado estado) {
  return (itens == estado.getItens());
}

bool Estado::existeItem(Item item) {
  for (Item i: itens) {
    if (i.cabeca == item.cabeca && i.prod == item.prod && i.posPonto == item.posPonto && i.lookahead == item.lookahead) {
      return true;
    }
  }
  return false;
}

void Estado::inserirItem(Item item) {
  if (!Estado::existeItem(item)) {
    itens.push_back(item);
  }
}

unsigned int Estado::getId() {
  return id;
}

std::vector<Item> Estado::getItens() {
  return itens;
}
