#pragma once

#include <string>
#include <vector>
#include "BuildOrder/GameState.h"
#include "BuildOrder/BuildItem.h"
#include "TechTree/TechTree.h"

#include "sc2api/sc2_unit.h"

namespace suboo {

std::string to_string(const UnitInstance::UnitState& state);
/* class GameState {
 std::vector<UnitInstance> freeUnits;
 std::vector<UnitInstance> attackUnits;
 std::vector<UnitInstance> busyUnits;

 float minerals;
 mutable float mps;
 float vespene;
 mutable float vps;
 int timestamp;
 mutable int supply;

public:
 GameState(const std::vector<UnitInstance>& units = {}, int minerals = 0,
           int vespene = 0)
     : freeUnits(units),
       minerals(minerals),
       mps(-1.0),
       vespene(vespene),
       vps(-1.0),
       timestamp(0),
       supply(-1) {}

 const std::vector<UnitInstance>& getFreeUnits() const { return freeUnits; }
 const std::vector<UnitInstance>& getAttackUnits() const {
   return attackUnits;
 }
 const std::vector<UnitInstance>& getBusyUnits() const { return busyUnits; }
 bool hasFreeUnit(UnitId unit) const;      // must be free;
 bool hasFinishedUnit(UnitId unit) const;  // must be finished;
 void addUnit(UnitId unit);
 void addUnit(const UnitInstance& unit);
 float getMineralsPerSecond() const;
 float getVespenePerSecond() const;
 float getMinerals() const { return minerals; }
 float& getMinerals() { return minerals; }
 float getVespene() const { return vespene; }
 float& getVespene() { return vespene; }
 int getAvailableSupply() const;
 int getUsedSupply() const;
 int getMaxSupply() const;
 void stepForward(int secs);
 bool waitForResources(int mins, int vesp,
                       std::pair<int, int>* waited = nullptr);
 bool waitforUnitCompletion(UnitId id);
 bool waitforUnitFree(UnitId id);
 bool waitforAllUnitFree();
 bool waitforFreeSupply(int needed);
 int probesToSaturation() const;
 bool assignProbe(UnitInstance::UnitState state);
 bool assignFreeUnit(UnitId type, UnitInstance::UnitState state, int time);
 int getTimeStamp() const { return timestamp; }
 void print(std::ostream& out) const;
 int countUnit(UnitId id) const;
 int countFreeUnit(UnitId unit) const;
}; */



class BuildOrder {
  GameState initial;
  std::deque<BuildItem> items;

  GameState final;

 public:
  void print(std::ostream& out);
  template <typename T>
  void addItem(T tocreate) {
    items.push_back(BuildItem(tocreate));
  }
  template <typename T>
  void addItemFront(T tocreate) {
    items.push_front(BuildItem(tocreate));
  }
  template <typename T>
  void insertItem(T tocreate, int index) {
    items.insert(items.begin() + index, BuildItem(tocreate));
  }
  const std::deque<BuildItem>& getItems() const { return items; }
  std::deque<BuildItem>& getItems() { return items; }
  GameState& getFinal() { return final; }
  const GameState& getFinal() const { return final; }
  void removeItem(int i) { items.erase(items.begin() + i); }
  void swapItems(int i, int j) { std::swap(items[i], items[j]); }
};

class BuildGoal {
  // using index of Unit to get desired quantity
  // std::vector<int> desiredPerUnit;
  std::unordered_map<int, int> desiredPerUnit;
  int framesToCompletion;  // 0 indicates ASAP
 public:
  BuildGoal(int deadline);
  void addUnit(UnitId id, int qty);
  void print(std::ostream& out) const;
  int getQty(int id) const { return desiredPerUnit.find(id)->second; }
  std::unordered_map<int, int> getMap() const { return desiredPerUnit; }
};

}  // namespace suboo