#include "Simulator/Simulator.h"

namespace suboo {

Simulator::Simulator(BuildOrder& bo)
    : bo(bo), gs(bo.getFinalState()), timeMinerals(0), timeVespene(0) {}

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

  //ressources
  if (u.mineral_cost > gs.getMinerals() && u.vespene_cost > gs.getVespene()) {
	  gs.waitForResources(u.mineral_cost, u.vespene_cost, nullptr);
  }

  //nourriture
  if (u.food_provided > gs.getMaxSupply()) {
	  gs.waitforFreeSupply(u.food_provided - gs.getMaxSupply());
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
  gs.getMinerals() -= u.mineral_cost;
  gs.getVespene() -= u.vespene_cost;
  gs.getSupply() -= u.food_provided;
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

void Simulator::visite(BIAWaitGoal& e) {
  gs.waitforAllUnitFree();
  e.setTime(gs.getTimeStamp());
}

void Simulator::execute() {
  for (BIA* bi : bo.getItems()) bi->accept(*this);
}

std::ostream& operator<<(std::ostream& os, const Simulator& simu) {
  os << "[Simulation]" << std::endl;
  os << "\t"
     << "TotalTime: " << simu.gs.getTimeStamp() << std::endl
     << "\t"
     << "timeMinerals: " << simu.timeMinerals << std::endl
     << "\t"
     << "timeVespene: " << simu.timeVespene << std::endl;

  os << simu.bo << std::endl;
  return os;
}

}  // namespace suboo
