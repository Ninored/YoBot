#include "Simulator/BuildOrder.h"
#include "Simulator/VisitorPrintBO.h"

namespace suboo {
BuildOrder::BuildOrder() {}
BuildOrder::BuildOrder(GameState init) : initial(init), final(init) {}
BuildOrder::BuildOrder(GameState init, GameState fin)
    : initial(init), final(fin) {}

BuildOrder::BuildOrder(const BuildOrder& b)
    : initial(b.initial), final(b.final) {
  for (const auto& i : b.items)
    items.push_back(std::unique_ptr<BIA>(i.get()->clone()));
}

BuildOrder BuildOrder::clone() const {
  BuildOrder out(initial, final);
  for (const auto& i : items)
    out.items.push_back(std::unique_ptr<BIA>(i.get()->clone()));
  return out;
}

void BuildOrder::addItem(BIA* item) {
  items.push_back(std::unique_ptr<BIA>(item));
}

void BuildOrder::addItem(const UnitId& id) {
  items.push_back(std::unique_ptr<BIABuild>(new BIABuild(id)));
}

void BuildOrder::addItem(const std::unique_ptr<BIA>& item) {
  items.push_back(std::unique_ptr<BIA>(item.get()->clone()));
}

void BuildOrder::addItemFront(BIA* item) {
  items.push_front(std::unique_ptr<BIA>(item));
}

void BuildOrder::addItemFront(const UnitId& id) {
  items.push_front(std::unique_ptr<BIABuild>(new BIABuild(id)));
}

void BuildOrder::addItemFront(const std::unique_ptr<BIA>& item) {
  items.push_back(std::unique_ptr<BIA>(item.get()->clone()));
}

void BuildOrder::insertItem(const UnitId& id, int index) {
  items.insert(items.begin() + index, std::unique_ptr<BIABuild>(new BIABuild(id)));
}

BIA* BuildOrder::extract(int i) { return items[i].get(); }

std::deque<std::unique_ptr<BIA>>& BuildOrder::getItems() { return items; }
const std::deque<std::unique_ptr<BIA>>& BuildOrder::getItems() const { return items; }

GameState& BuildOrder::getInitialState() { return initial; }
const GameState& BuildOrder::getInitialState() const { return initial; }

GameState& BuildOrder::getFinalState() { return final; }
const GameState& BuildOrder::getFinalState() const { return final; }

void BuildOrder::removeItem(int i) { items.erase(items.begin() + i); }

void BuildOrder::swapItems(int i, int j) { std::swap(items[i], items[j]); }

void BuildOrder::moveFirst(int i) {
  auto it = items[i].get()->clone();
  items.erase(items.begin() + i);
  addItemFront(it);
}

void BuildOrder::moveLast(int i) {
  auto it = items[i].get()->clone();
  items.erase(items.begin() + i);
  addItem(it);
}

BuildOrder BuildOrder::operator=(const BuildOrder& b) {
  BuildOrder out(initial, final);
  for (auto& i : b.items) out.addItem(i.get()->clone());

	return out;
}

std::ostream& operator<<(std::ostream& os, const BuildOrder& bo) {
  os << "[BuildOrder]" << std::endl;
  BuildOrder bb(bo.clone());
  os << PrintBo(bb) << std::endl;

  return os;
}
}  // namespace suboo