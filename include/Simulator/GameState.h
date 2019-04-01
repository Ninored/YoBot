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

  float vespenes;
  float vps;

  int timestamp;

  int supply;


  void calculateMps();
  void calculateVps();
  void calculateSupply();


 public:
  GameState(const std::vector<UnitInstance> & units, int m, int v);
  friend std::ostream& operator<<(std::ostream& os, const GameState& state);

  // Step function
  void step(int sec);

  // GET
  int getAvailabelSupply() const;
  int getMaxSupply() const;
  int getTimeStamp() const;
  float& getMinerals();
  float getMps() const;
  float& getVespene();
  float getVps() const;
  const std::list<UnitInstance*>& getFreeUnits() const;
  const std::list<UnitInstance*>& getBusyUnits() const;
  
  // Set
  void setFree(UnitInstance* inst);

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
  std::pair<int, int> waitForRessources(int minerals, int vespene);

  // Count
  int countUnit(UnitId id) const;
  int countFreeUnit(UnitId id) const;

  // Tools
  int probesToStauration() const;
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_GAMESTATE
