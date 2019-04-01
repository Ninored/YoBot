#include "Simulator/BuildOrder.h"

namespace suboo {
BuildOrder::BuildOrder(GameState init): initial(init), final(init) {}

void BuildOrder::addItem(BIA* item) { items.push_back(item); }

void BuildOrder::pushFront(BIA* i) { items.push_front(i); }

BIA* BuildOrder::extract(int i) { return items[i]; }

std::deque<BIA*>& BuildOrder::getItems() { return items; }

GameState& BuildOrder::getFinalState() { return final; }

void BuildOrder::removeItem(int i) {}

void BuildOrder::swapItem(int i, int j) { std::swap(items[i], items[j]); }

void BuildOrder::moveFirst(int i) {}

void BuildOrder::moveLast(int i) {}

std::ostream& operator<<(std::ostream& os, const BuildOrder& bo) { 
  os << "[BuildOrder]" << std::endl;
  /*
  for (BIA* i : bo.items) {
    os << "\t" << *i << std::endl;
  }
  */
  return os;
}
}  // namespace suboo