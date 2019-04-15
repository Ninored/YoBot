#include "Simulator/BuildOrderGoal.h"
namespace suboo {

BuildOrderGoal::BuildOrderGoal() {}

void BuildOrderGoal::addUnit(UnitId id, int qty) {
  auto& it = quantityPerUnits.insert({(int)id, 0});
  it.first->second += qty;
}

const std::unordered_map<int, int>& BuildOrderGoal::getMap() const {
  return quantityPerUnits;
}

std::ostream& operator<<(std::ostream& os, const BuildOrderGoal& goal) {
  os << "[BuildOrderGoal]" << std::endl;
  for (auto& i : goal.quantityPerUnits)
    os << "\t" << sc2::UnitTypeToName(i.first) << ": " << i.second << std::endl;
  os << std::endl;

  return os;
}
}  // namespace suboo
