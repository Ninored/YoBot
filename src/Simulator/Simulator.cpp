#include "Simulator/Simulator.h"

namespace suboo {

Simulator::Simulator(BuildOrder& bo)
    : bo(bo), gs(bo.getFinalState()), timeMinerals(0), timeVespene(0) {}

void Simulator::visite(BIABuild& e) {
  auto tt = TechTree::getTechTree();
  auto u = tt.getUnit(e.getTarget());

  std::cout << "==========================================="  << std::endl << u << std::endl;

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
  gs.getMinerals() -= u.mineral_cost;
  gs.getVespene() -= u.vespene_cost;
  gs.addUnit(UnitInstance(u.id, UnitInstance::BUILDING, u.production_time));

  e.setTime(gs.getTimeStamp());
  std::cout << "GS: " << gs << std::endl;
}

void Simulator::visite(BIAWaitGoal& e) { 
  gs.waitforAllUnitFree(); 
  e.setTime(gs.getTimeStamp());
}


void Simulator::execute() {
  for (BIA*  bi : bo.getItems()) bi->accept(*this);
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
