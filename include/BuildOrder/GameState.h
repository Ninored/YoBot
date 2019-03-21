#pragma once

#include <vector>
#include "TechTree/TechTree.h"

namespace suboo {
class GameState {
  std::vector<UnitInstance> freeUnits;
  std::vector<UnitInstance> attackUnits;
  std::vector<UnitInstance> busyUnits;

  float minerals;
  mutable float mps;

  float vespene;
  mutable float vps;

  mutable int supply;
  int timestamp;

 public:
  GameState(const std::vector<UnitInstance>& units = {}, int minerals = 0,
            int vespene = 0);

  // Step
  void stepForward(int secs);

  // GET
  int getAvailableSupply() const;
  int getUsedSupply() const;
  int getMaxSupply() const;
  int getTimeStamp() const { return timestamp; }
  float getMineralsPerSecond() const;
  float getVespenePerSecond() const;
  float getMinerals() const { return minerals; }
  float& getMinerals() { return minerals; }
  float getVespene() const { return vespene; }
  float& getVespene() { return vespene; }
  const std::vector<UnitInstance>& getFreeUnits() const { return freeUnits; }
  const std::vector<UnitInstance>& getAttackUnits() const {
    return attackUnits;
  }

  const std::vector<UnitInstance>& getBusyUnits() const { return busyUnits; }

  // HAS
  bool hasFreeUnit(UnitId unit) const;      // must be free;
  bool hasFinishedUnit(UnitId unit) const;  // must be finished;

  // ADD
  void addUnit(UnitId unit);
  void addUnit(const UnitInstance& unit);
  bool waitForResources(int mins, int vesp,
                        std::pair<int, int>* waited = nullptr);

  // Wait
  bool waitforUnitCompletion(UnitId id);
  bool waitforUnitFree(UnitId id);
  bool waitforAllUnitFree();
  bool waitforFreeSupply(int needed);

  // Assign
  bool assignProbe(UnitInstance::UnitState state);
  bool assignFreeUnit(UnitId type, UnitInstance::UnitState state, int time);

  // Count
  int countUnit(UnitId id) const;
  int countFreeUnit(UnitId unit) const;

  int probesToSaturation() const;

  // print
  void print(std::ostream& out) const;
};

}  // namespace suboo