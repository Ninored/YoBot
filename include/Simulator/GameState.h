#ifndef INCLUDE_SIMULATOR_GAMESTATE
#define INCLUDE_SIMULATOR_GAMESTATE

#include <iostream>
#include <list>
#include <vector>
#include "TechTree/TechTree.h"

namespace suboo {
class GameState {
  std::vector<UnitInstance> freeUnits;
  std::vector<UnitInstance> attackUnits;
  std::vector<UnitInstance> busyUnits;

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
  GameState();
  GameState(const std::vector<UnitInstance> & units, int m, int v);
  friend std::ostream& operator<<(std::ostream& os, const GameState& state);

  // Step function
  void step(int sec);

  // GET
  int getAvailableSupply() const;
  int getMaxSupply() const;
  int getTimeStamp() const;
  float& getMinerals();
  float getMinerals() const;
  float getMps() const;
  float& getVespene();
  float getVespene() const;
  float getVps() const;
  std::vector<UnitInstance>& getFreeUnits();
  std::vector<UnitInstance>& getBusyUnits();
  
  // Set
  void setFree(std::vector<UnitInstance>::iterator& it);
  void setBusy(std::vector<UnitInstance>::iterator& it);

  // Mutate unit
  bool addUnit(UnitId unit);
  bool addUnit(const UnitInstance& unit);
  bool assignProbe(UnitInstance::UnitState state);
  bool assignFreeUnit(UnitId type, UnitInstance::UnitState state, int time);

  // Has
  bool hasFreeUnit(UnitId unit) const;
  bool hasFinishedUnit(UnitId unit) const;

  // Wait function
  bool waitforUnitFree(UnitId id);
  bool waitforUnitCompletion(UnitId id);
  bool waitforFreeSupply(int nedded);
  bool waitforAllUnitFree();
  std::pair<int, int> waitForRessources(int minerals, int vespene);

  // Count
  int countUnit(UnitId id) const;
  int countFreeUnit(UnitId id) const;

  // Tools
  int probesToSaturation() const;
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_GAMESTATE
