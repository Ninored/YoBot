#include "Simulator/BuildItem.h"

namespace suboo {

BuildItem::BuildItem(Action a, UnitId t) : action(a), target(t), time(0) {}

BuildItem::Action BuildItem::getAction() const { return action; }
UnitId BuildItem::getTarget() const { return target; }
int BuildItem::getTime() const { return time; }

void BuildItem::setTime(int val) { time = val; }
bool BuildItem::operator==(const BuildItem& other) const {
  return action == other.action&& target == other.target;
}

void BuildItem::execute(GameState& s) {
  std::cout << "exec" << std::endl;
  switch (action) {
    case BUILD:
      break;
    case TRANSFER_VESPENE:
      break;
    case TRANSFER_MINERALS:
      break;
    case WAIT_GOAL:
      break;
    case CHRONOBOOST:
      break;
    default:
      break;
  }
}

std::ostream& operator<<(std::ostream& os, const BuildItem& i) {
  std::string action_name;

  switch (i.action) {
    case BuildItem::BUILD:
      action_name = "BUILD";
      break;
    case BuildItem::TRANSFER_VESPENE:
      action_name = "TRANSFER_VESPENE";
      break;
    case BuildItem::TRANSFER_MINERALS:
      action_name = "TRANSFER_MINERALS";
      break;
    case BuildItem::WAIT_GOAL:
      action_name = "WAIT_GOAL";
      break;
    case BuildItem::CHRONOBOOST:
      action_name = "CHRONOBOOST";
      break;
    default:
      action_name = "UNDEFINED";
      break;
  }
  os << "[" << action_name << "]: " << sc2::UnitTypeToName(i.target);
  return os;
}
}  // namespace suboo