#include "Simulator/Simulator.h"

namespace suboo {

Simulator::Simulator()
    : timeMinerals(0), timeVespene(0) {}

void Simulator::visite(BIABuild& e) {
  auto tt = TechTree::getTechTree();
  auto u = tt.getUnit(e.getTarget());

  std::cout << "===========================================" << std::endl
            << u << std::endl;

  std::pair<int, int> waited;
  int current = gs.getTimeStamp();

  waited = gs.waitForRessources(u.mineral_cost, u.vespene_cost);
  timeMinerals += waited.first;
  timeVespene += waited.second;

  /* Prereq */
  if (u.prereq != 0 && !gs.hasFreeUnit(u.prereq)) {
    gs.waitforUnitCompletion(u.prereq);
  }
  /* Builder */
  if (!gs.hasFreeUnit(u.builder)) {
    gs.waitforUnitFree(u.builder);
    if (u.action_status == u.TRAVELLING) {
      gs.assignFreeUnit(u.builder, UnitInstance::BUSY, u.travel_time);
    } else if (u.action_status == u.BUSY) {
      gs.assignFreeUnit(u.builder, UnitInstance::BUSY, u.production_time);
    }
  }

  if (u.food_provided < 0 && gs.getAvailabelSupply() < -u.food_provided) {
    if (!gs.waitforFreeSupply(-u.food_provided)) {
      std::cout << "Insufficient food missing pylons." << std::endl;
      // gs.print(std::cout);
      return;
    }
    //bi.timeFood = gs.getTimeStamp() - cur;
  }

  gs.getMinerals() -= u.mineral_cost;
  gs.getVespene() -= u.vespene_cost;
  gs.addUnit(UnitInstance(u.id, UnitInstance::BUILDING, u.production_time));

  e.setTime(gs.getTimeStamp());
  std::cout << "GS: " << gs << std::endl;
}

void Simulator::visite(BIAMineVespene& e) {
  auto prereq = sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR;
  if (!gs.hasFreeUnit(prereq)) {
    if (!gs.waitforUnitCompletion(prereq)) {
      std::cerr << "[ERROR] No assimilator in BuildOrder" << std::endl;
      return;
    }
  }
  int gas = 0;
  int soongas = 0;
  int vcount = 0;

  for (auto& u : gs.getFreeUnits()) {
    if (u.state == UnitInstance::MINING_VESPENE) {
      vcount++;
    } else if (u.type == sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR &&
               u.state == UnitInstance::FREE) {
      gas++;
    }
  }
  for (auto& u : gs.getBusyUnits()) {
    if (u.state == UnitInstance::MINING_VESPENE) {
      vcount++;
    } else if (u.type == sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) {
      soongas++;
    }
  }

  if (vcount >= 3 * (gas + soongas)) {
    std::cerr << "Gas over saturated skipping \n";
    return;
  }
  if (vcount >= 3 * gas) {
    if (!gs.waitforUnitCompletion(prereq)) {
      std::cerr << "No assimilator in state \n";
      return;
    }
  }
  if (!gs.assignProbe(UnitInstance::MINING_VESPENE)) {
    std::cerr << "No probe available for mining \n";
    return;
  }
}

void Simulator::visite(BIAChronoboost& e) {
  std::cout << "[CHRONOBOOST]" << std::endl;

  auto& fu = gs.getFreeUnits();
  auto& bu = gs.getBusyUnits();

  auto tt = TechTree::getTechTree();
  auto u = tt.getUnit(e.getTarget());

  UnitId target = e.getTarget();
  UnitId builder = u.builder;

  /* Search Nexus */
  auto nexus = std::find_if(fu.begin(), fu.end(), [](auto& u) {
    return u.type == sc2::UNIT_TYPEID::PROTOSS_NEXUS;
  });
  if (nexus == fu.end()) {
    std::cerr << "No nexus available for Chronoboost" << std::endl;
    return;
  }
  if (nexus->energy < 50) {
    std::cerr << "No energy in nexus" << std::endl;
    return;
  }

  /* Search target */
  auto concret_target = std::find_if(
      bu.begin(), bu.end(), [target](auto& u) { return u.type == target; });
  if (concret_target == bu.end()) {
    std::cerr << "No target available" << std::endl;
    return;
  }

  auto concret_builder = std::find_if(
      bu.begin(), bu.end(), [builder](auto& u) { return u.type == builder; });
  if (concret_builder == bu.end()) {
    std::cerr << "No builder available" << std::endl;
    return;
  }

  nexus->energy -= 50;
  concret_builder->time_with_chronoboost = 20;
  concret_target->time_with_chronoboost = 20;
}

void Simulator::visite(BIAWaitGoal& e) {
  gs.waitforAllUnitFree();
  e.setTime(gs.getTimeStamp());
}

GameState Simulator::visite(BuildOrder& bo) {
	for (const auto& bi : bo.getItems()) bi.get()->accept(*this);
	gs = bo.getFinalState();
	return gs;
}

std::ostream& operator<<(std::ostream& os, const Simulator& simu) {
  os << "[Simulation]" << std::endl;
  os << "\t"
     << "TotalTime: " << simu.gs.getTimeStamp() << std::endl
     << "\t"
     << "timeMinerals: " << simu.timeMinerals << std::endl
     << "\t"
     << "timeVespene: " << simu.timeVespene << std::endl;

  os << simu.gs << std::endl;
  return os;
}

}  // namespace suboo
