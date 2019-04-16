#include "Simulator/BuildOrder.h"
#include "Simulator/VisitorPrintBO.h"

namespace suboo {
BuildOrder::BuildOrder(GameState init) : initial(init), final(init) {}
BuildOrder::BuildOrder(GameState init, GameState fin)
    : initial(init), final(fin) {}

BuildOrder BuildOrder::clone() const {
  BuildOrder out(initial, final);
  for (const auto& i : items)
    out.items.push_back(std::unique_ptr<BIA>(i.get()->clone()));
  return out;
}

void BuildOrder::addItem(BIA* item) {
  items.push_back(std::unique_ptr<BIA>(item));
}

void BuildOrder::pushFront(BIA* i) {
  items.push_front(std::unique_ptr<BIA>(i));
}

BIA* BuildOrder::extract(int i) { return items[i].get(); }

std::deque<std::unique_ptr<BIA>>& BuildOrder::getItems() { return items; }

GameState& BuildOrder::getFinalState() { return final; }

void BuildOrder::removeItem(int i) {}

void BuildOrder::swapItem(int i, int j) { std::swap(items[i], items[j]); }

void BuildOrder::moveFirst(int i) {}

void BuildOrder::moveLast(int i) {}

BuildOrder BuildOrder::operator=(const BuildOrder& b) { return this->clone(); }

std::ostream& operator<<(std::ostream& os, const BuildOrder& bo) {
  os << "[BuildOrder]" << std::endl;
  BuildOrder bb(bo.clone());
  os << PrintBo(bb) << std::endl;

  return os;
}
}  // namespace suboo