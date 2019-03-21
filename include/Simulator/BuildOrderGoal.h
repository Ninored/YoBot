#ifndef INCLUDE_SIMULATOR_BUILDORDERGOAL
#define INCLUDE_SIMULATOR_BUILDORDERGOAL

#include <unordered_map>
#include "GameState.h"
#include "TechTree/TechTree.h"

namespace suboo {

class BuildOrderGoal {
  std::unordered_map<int, int> quantityPerUnits;

 public:
  BuildOrderGoal();

  void addUnit(UnitId id, int qty);
  const std::unordered_map<int, int>& getMap() const;

  friend std::ostream& operator<<(std::ostream& os, const BuildOrderGoal& goal);
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_BUILDORDERGOAL
