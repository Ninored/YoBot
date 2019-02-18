#ifndef TECHTREE_H
#define TECHTREE_H

#include <unordered_map>
#include <vector>
#include "sc2api/sc2_data.h"
#include "sc2api/sc2_unit.h"

namespace suboo {

using UnitId = sc2::UnitTypeID;   // UnitId using
using AbilityId = sc2::AbilityID; // AbilityId using

/**
 * Unit Class
 */
class Unit {
 public:
  UnitId id;       // Unit id
  std::string name;  // Unit name
  int mineral_cost;  // Unit mineral costs
  int vespene_cost;  // Unit vespene costs
  int food_cost;     // Unit food costs (can be positive)
  std::vector<AbilityId> abilities;  // Abilities of the unit

  UnitId requirement;  // Tech requirement

  int production_time;  // Unit creation time
  int travel_time;      // Unit travel time

  enum Status { BUSY, TRAVELLING, CONSUMING };
  Status action_status;  // Unit action behavior
  
  // Operators
  std::size_t operator()(const Unit& k) const;
  bool operator==(const Unit& othre) const;

  // Firends
  friend std::ostream& operator<<(std::ostream& os, const Unit& u);
};

/**
 *  UnitInstance class
 */
class UnitInstance {
 public:
  UnitId unittype;
  enum UnitState { BUILDING, BUSY, MINING_MINERALS, MINING_VESPENE, FREE };
  UnitState state;
  int time_to_free;
  UnitInstance(UnitId id) : unittype(id){};
  UnitInstance(UnitId id, UnitState state, int time)
      : unittype(id), state(state), time_to_free(time){};
};

/**
 * Singleton class used to query information about units
 * It can be serialized and deserialized
 */
class TechTree {
  std::vector<Unit> units;
  std::vector<UnitId> index;
  std::unordered_map<int, Unit> map;
  int version;
  TechTree();

 public:
  friend std::ostream& operator<<(std::ostream& os, const TechTree& t);
  static TechTree& getTechTree();
  const Unit& getUnit(UnitId) const;
  const std::unordered_map<int, Unit>& getMap() const { return map; };
  void addUnit(Unit u);
  std::string serialize();
};
}  // namespace suboo

#endif  // TECHTREE_H