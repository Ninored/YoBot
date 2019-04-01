#include "Simulator/Simulator.h"

namespace suboo {

Simulator::Simulator(BuildOrder& bo)
    : bo(bo), gs(bo.getFinalState()), timeMinerals(0), timeVespene(0) {}

void Simulator::visite(BIABuild& e) {
  auto tt = TechTree::getTechTree();
  auto u = tt.getUnit(e.getTarget());
  std::pair<int, int> waited;
  int current = gs.getTimeStamp();

  waited = gs.waitForRessources(u.mineral_cost, u.vespene_cost);
  timeMinerals += waited.first;
  timeVespene += waited.second;

  if (!gs.hasFreeUnit(u.builder)) {
    gs.waitforUnitFree(u.builder);

    if (u.action_status == u.TRAVELLING) {
      gs.assignFreeUnit(u.builder, UnitInstance::BUSY, u.action_status);
    } else if (u.action_status == u.BUSY) {
      gs.assignFreeUnit(u.builder, UnitInstance::BUSY, u.production_time);
    }
  }
  gs.getMinerals() -= u.mineral_cost;
  gs.getVespene() -= u.vespene_cost;
  gs.addUnit(u.id);
  e.setTime(gs.getTimeStamp());
}



void Simulator::execute() {
  for (BIA*  bi : bo.getItems()) bi->accept(*this);
}
/*

void Simulator::exec(BuildItem& item) {
  auto tt = TechTree::getTechTree();
  auto u = tt.getUnit(item.getTarget());
  std::pair<int, int> waited;
  int current = gs.getTimeStamp();

  switch (item.getAction()) {
    case BuildItem::TRANSFER_VESPENE:
      break;
    case BuildItem::TRANSFER_MINERALS:
      break;
    case BuildItem::WAIT_GOAL:
      break;
    case BuildItem::CHRONOBOOST:
      break;

    default:
      std::cerr << "Unhandled action" << std::endl;
      break;
  }
  item.setTime(gs.getTimeStamp());
}
*/
std::ostream& operator<<(std::ostream& os, const Simulator& simu) {
  os << "[Simulation]" << std::endl;
  os << "\t"
     << "TotalTime: " << simu.gs.getTimeStamp() << std::endl
     << "\t"
     << "timeMinerals: " << simu.timeMinerals << std::endl
     << "\t"
     << "timeVespene: " << simu.timeVespene << std::endl;
  return os;
}

}  // namespace suboo
