#include "Simulator/BuildOrder.h"

namespace suboo {
BuildOrder::BuildOrder() {}

void BuildOrder::addItem(BuildItem item) { items.push_back(item); }

void BuildOrder::pushFront(BuildItem i) { items.push_front(i); }

BuildItem BuildOrder::extract(int i) { return items[i]; }

const std::deque<BuildItem>& BuildOrder::getItems() const { return items; }

const GameState& BuildOrder::getFinalState() const { return final; }

void BuildOrder::removeItem(int i) {}

void BuildOrder::swapItem(int i, int j) { std::swap(items[i], items[j]); }

void BuildOrder::moveFirst(int i) {}

void BuildOrder::moveLast(int i) {}

void BuildOrder::execute() {
  for (BuildItem i : items) {
    i.execute(final);
  }
}

std::ostream& operator<<(std::ostream& os, const BuildOrder& bo) { 
  os << "[BuildOrder]" << std::endl;
  for (BuildItem i : bo.items) {
    os << "\t" << i << std::endl;
  }
  return os;
}
}  // namespace suboo