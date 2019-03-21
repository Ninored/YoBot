#ifndef INCLUDE_SIMULATOR_GAMESTATE
#define INCLUDE_SIMULATOR_GAMESTATE

#include <iostream>
#include <list>
#include <vector>
#include "TechTree/TechTree.h"

namespace suboo {
class GameState {
  std::vector<UnitInstance> allUnits;
  std::list<UnitInstance*> freeUnits;
  std::list<UnitInstance*> busyUnits;

  float minerals;
  float mps;

  float vespene;
  float vps;

  int timestamp;

  int supply;

 public:
  GameState();
  friend std::ostream& operator<<(std::ostream& os, const GameState& state);

  // Step function
  void step(int sec);

  // GET
  int getAvailabelSupply() const;
  int getMaxSupply() const;
  int getTimeStamp() const;
  float getMinerals() const;
  float getMps() const;
  float getVespene() const;
  float getVps() const;
  const std::list<UnitInstance*>& getFreeUnits() const;
  const std::list<UnitInstance*>& getBusyUnits() const;

  // Mutate unit
  bool addUnit(UnitId unit);
  bool addUnit(const UnitInstance& unit);
  bool assignProbe(UnitInstance::UnitState state);
  bool assignFreeUnit(UnitId type, UnitInstance::UnitState state, int time);

  // Has
  bool hasFreeUnit(UnitId unit) const;

  // Wait function
  bool waitforUnitFree(UnitId id);
  bool waitforFreeSupply(int nedded);
  bool waitforAllUnitFree();

  // Count
  int countUnit(UnitId id) const;
  int countFreeUnit(UnitId id) const;

  // Tools
  int probesToStauration() const;
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_GAMESTATE
