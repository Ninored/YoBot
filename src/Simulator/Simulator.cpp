#include "Simulator/Simulator.h"

namespace suboo {

Simulator::Simulator(GameState& initial)
    : gs(initial) {}

void Simulator::visite(BIABuild& e) {
  auto tt = TechTree::getTechTree();
  auto u = tt.getUnit(e.getTarget());

  std::cout << "===========================================" << std::endl
            << u << std::endl;

  std::pair<int, int> waited;
  int current = gs.getTimeStamp();

  waited = gs.waitForRessources(u.mineral_cost, u.vespene_cost);
  e.timeMinerals += waited.first;
  e.timeVespene += waited.second;

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
    e.timeFreeUnit = gs.getTimeStamp() - current;
  }

  if (u.food_provided < 0 && gs.getAvailabelSupply() < -u.food_provided) {
    if (!gs.waitforFreeSupply(-u.food_provided)) {
      throw std::runtime_error("Insufficient food, missing pylons.");
    }
    e.timeFood = gs.getTimeStamp() - current;
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
      throw std::runtime_error("[ERROR] No assimilator in BuildOrder");
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
    throw std::runtime_error("Gas over saturated skipping");
  }
  if (vcount >= 3 * gas) {
    if (!gs.waitforUnitCompletion(prereq)) {
      throw std::runtime_error("No assimilator in state");
    }
  }
  if (!gs.assignProbe(UnitInstance::MINING_VESPENE)) {
    throw std::runtime_error("No probe available for mining");
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
    throw std::runtime_error("No nexus available for Chronoboost");
  }
  if (nexus->energy < 50) {
    throw std::runtime_error("No energy in nexus");
  }

  /* Search target */
  auto concret_target = std::find_if(
      bu.begin(), bu.end(), [target](auto& u) { return u.type == target; });
  if (concret_target == bu.end()) {
    throw std::runtime_error("No target available");
  }

  auto concret_builder = std::find_if(
      bu.begin(), bu.end(), [builder](auto& u) { return u.type == builder; });
  if (concret_builder == bu.end()) {
    throw std::runtime_error("No builder available");
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
  return gs;
}

std::ostream& operator<<(std::ostream& os, const Simulator& simu) {
  os << "[Simulation]" << std::endl;
  os << "\t"
     << "TotalTime: " << simu.gs.getTimeStamp() << std::endl;
	
  os << simu.gs << std::endl;
  return os;
}

}  // namespace suboo
