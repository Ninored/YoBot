#pragma once

#include <sc2api/sc2_unit.h>
#include "BuildOrder/GameState.h"

namespace suboo {

class BuildItem {
 public:
  enum BuildAction {
    BUILD,
    TRANSFER_VESPENE,
    TRANSFER_MINERALS,
    WAIT_GOAL,
    CHRONO
  };

 private:
  BuildAction action;

  UnitId target;
  int time;

 public:
  int timeMin = 0, timeVesp = 0, timePre = 0, timeFree = 0, timeFood = 0;
  int totalWait() const {
    return timeMin + timeVesp + timePre + timeFree + timeFood;
  }
  void clearTimes() { timeMin = timeVesp = timePre = timeFree = timeFood = 0; }
  // compute time to completion, from a given game state, assuming that the
  // action is possible make sure this is the case, since this just steps
  // simulation forward updates the game state
  void executeBuildItem(GameState& s) {}

  void print(std::ostream& out) const {
    if (time != 0) {
      out << time / 60 << ":" << time % 60 << " (" << time << " s)";
    }
    auto& tech = TechTree::getTechTree();
    if (action == BUILD) {
      out << "Build " << tech.getUnit((int)target).name;
    } else if (action == TRANSFER_VESPENE) {
      out << "Transfer To Vespene";
    } else if (action == TRANSFER_MINERALS) {
      out << "Transfer To Minerals";
    } else if (action == WAIT_GOAL) {
      out << "Wait for Goal";
    }
    out << " waited : ";
    if (timeMin > 0) out << " min: " << timeMin;
    if (timeVesp > 0) out << " vesp: " << timeVesp;
    if (timePre > 0) out << " pre: " << timePre;
    if (timeFree > 0) out << " free: " << timeFree;
    if (timeFood > 0) out << " food: " << timeFood;
  }

  BuildItem(UnitId id) : action(BUILD), target(id), time(0) {}
  BuildItem(BuildAction action)
      : action(action), target(sc2::UNIT_TYPEID::INVALID), time(0) {}

  BuildAction getAction() const { return action; }
  UnitId getTarget() const { return target; }
  void setTime(int ttime) { time = ttime; }
  int getTime() const { return time; }
  bool operator==(const BuildItem& other) const {
    return action == other.action && target == other.target;
  }
  bool operator==(const UnitId& other) const { return other == target; }
};
}  // namespace suboo