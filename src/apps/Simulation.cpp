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
  tech.deserialize("techtree_72282.json");

  BuildOrder bo(GameState(tech.getInitialUnits(), tech.getInitialMinerals(),
                          tech.getInitialVespene()));

  /*
  YoBot(legacy):  95 sec
  YoBot(new):  95 sec
  PROBOEngine: 74 sec with chronoboost
  YoBot(Chronoboost): 78 sec
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PYLON));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_GATEWAY));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_ZEALOT));
  bo.addItem(new BIAChronoboost(sc2::UNIT_TYPEID::PROTOSS_ZEALOT));
  bo.addItem(new BIAWaitGoal());
  */

  /*
  PROBO Engine: 2min 48 with chronoboost
  YOBOT (legacy): 3 min 21
  YOBOT (new): 3 min 21
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PYLON));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_GATEWAY));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR));
  bo.addItem(new BIAMineVespene());
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_STARGATE));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PHOENIX));
  bo.addItem(new BIAChronoboost(sc2::UNIT_TYPEID::PROTOSS_PHOENIX));
  bo.addItem(new BIAWaitGoal());
  */

	// PROBO engine: 1 min 50
	// YOBOT(new): 1min 30 
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_GATEWAY));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PYLON));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_ZEALOT));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_ZEALOT));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_ZEALOT));
  bo.addItem(new BIAWaitGoal());

  std::cout << bo << std::endl;

  Simulator simu(GameState(tech.getInitialUnits(), tech.getInitialMinerals(),
                           tech.getInitialVespene()));

  try {
    std::cout << simu.visite(bo) << std::endl;
    std::cout << "=============" << std::endl;
    std::cout << bo << std::endl;

  } catch (const std::exception ex) {
    std::cerr << ex.what() << std::endl;
	}

  return 0;
}