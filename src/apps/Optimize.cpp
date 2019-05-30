#include "Optimizer/Opti.h"
#include "TechTree/TechTree.h"
#include "Simulator/BuildOrder.h"
#include "Simulator/GameState.h"
#include "Simulator/Simulator.h"
#include "Optimizer/Prereq.h"

using namespace std;
using namespace suboo;

int main(int argc, char* argv[]) {
  auto& tech = TechTree::getTechTree();
  tech.deserialize("techtree_72282.json");

  BuildOrder bo(GameState(tech.getInitialUnits(), tech.getInitialMinerals(),
                          tech.getInitialVespene()));

  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PYLON));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_GATEWAY));
  bo.addItem(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_ZEALOT));
  //bo.addItem(new BIAChronoboost(sc2::UNIT_TYPEID::PROTOSS_ZEALOT));
  bo.addItem(new BIAWaitGoal());

//	auto op = EnforcePrereq(bo).visite(bo);

  auto op = improveBO(bo, 2);

  Simulator si(op.getInitialState());

  cout << si.visite(op) << endl;

  return 0;
}
