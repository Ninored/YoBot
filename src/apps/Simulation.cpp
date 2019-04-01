#include <iostream>
#include "Simulator/BuildOrder.h"
#include "Simulator/BuildOrderGoal.h"
#include "Simulator/GameState.h"
#include "Simulator/Simulator.h"
#include "TechTree/TechTree.h"

using namespace suboo;

int main(int argc, char** argv) {
  // Loading techtree
  auto& tech = TechTree::getTechTree();
  tech.deserialize("techtree_.json");

  BuildOrder bo(GameState(tech.getInitialUnits(), tech.getInitialMinerals(),
                          tech.getInitialVespene()));
  BuildOrderGoal goal;
  goal.addUnit(sc2::UNIT_TYPEID::PROTOSS_IMMORTAL, 4);
  std::cout << goal << std::endl;

  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PROBE));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PROBE));
  std::cout << bo << std::endl;

  Simulator simu(bo);
  simu.execute();

  std::cout << simu << std::endl;
  std::cout << bo << std::endl;

  return 0;
}