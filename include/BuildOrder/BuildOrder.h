#pragma once

#include <string>
#include <vector>
#include "TechTree/TechTree.h"
#include "sc2api/sc2_unit.h"

namespace suboo {

std::string to_string(const UnitInstance::UnitState& state);
class GameState {
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
};

/*
        class TechTree {
                std::vector<Unit> units;
                GameState initial;
                std::vector<int> indexes; // correspondance from UnitId to unit
   index TechTree(); public:
                // singleton
                static const TechTree & getTechTree();
                int TechTree::getUnitIndex(UnitId id) const
                {
                        return indexes[(int)id];
                }
                const Unit & TechTree::getUnitById(UnitId id) const
                {
                        return units[getUnitIndex(id)];
                }
                const Unit & TechTree::getUnitByIndex(int index) const
                {
                        return units[index];
                }
                TechTree(const TechTree &) = delete;
                size_t size() const { return indexes.size(); }
                const GameState & getInitial() const { return initial; }
        };
  */

enum BuildAction {
  BUILD,
  TRANSFER_VESPENE,
  TRANSFER_MINERALS,
  WAIT_GOAL,
  CHRONO
};

class BuildItem {
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
  void executeBuildItem(GameState& s);
  void print(std::ostream& out) const;

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