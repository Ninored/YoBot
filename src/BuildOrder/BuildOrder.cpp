#include "BuildOrder/BuildOrder.h"
#include <iostream>
#include <mutex>
#include "BuildOrder/UnitTypes.h"
#include "TechTree/TechTree.h"

namespace suboo {
BuildGoal::BuildGoal(int deadline)
    : framesToCompletion(deadline),
      desiredPerUnit(TechTree::getTechTree().getMap().size()) {}

void BuildGoal::addUnit(UnitId id, int qty) {
  auto &it = desiredPerUnit.insert({(int)id, 0});
  it.first->second += 1;
}

void BuildGoal::print(std::ostream &out) const {
  auto &tech = TechTree::getTechTree().getMap();
  out << "Goal time=" << framesToCompletion << std::endl;
  for (auto &it : desiredPerUnit) {
    out << it.second << " x " << tech.find(it.first)->second.name << std::endl;
  }
}

void BuildOrder::print(std::ostream &out) {
  int step = 1;
  for (auto &bi : items) {
    out << step++ << ":";
    bi.print(out);
    out << std::endl;
  }
  out << "Reached state at " << final.getTimeStamp() / 60 << "m"
      << final.getTimeStamp() % 60 << "s" << std::endl;
  final.print(out);
}

void UnitInstance::print(std::ostream &out) const {
  out << TechTree::getTechTree().getUnit(type).name;
  std::string status = to_string(state);

  out << "(" << status;
  if (time_to_free > 0) out << time_to_free;
  out << ")";
}

}  // namespace suboo