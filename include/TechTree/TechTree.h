#ifndef TECHTREE_H
#define TECHTREE_H

#include <vector>
#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_unit.h"

namespace suboo {

using UnitId = sc2::UnitTypeID;  // Type using

/**
 * Unit Class
 */
class Unit {
 public:
  UnitId type;       // Unit id
  std::string name;  // Unit name
  int mineral_cost;  // Unit mineral costs
  int vespene_cost;  // Unit vespene costs
  int food_cost;     // Unit food costs (can be positive)

  UnitId builder;                    // Unit required to build
  std::vector<UnitId> requirements;  // Tech requirement

  int production_time;  // Unit creation time
  int travel_time;      // Unit travel time

  enum Status { BUSY, TRAVELLING, CONSUMING };
  Status acton_status;  // Unit action behavior
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
 * @brief Singleton class used to query information about units
 * It can be serialized and deserialized
 */
class TechTree {
  std::vector<Unit> units_vec;
  sc2::Agent agent;
  TechTree();
  
 public:
  static TechTree& getTechTree();
  sc2::Agent* getAgent();
};
}  // namespace suboo

#endif  // TECHTREE_H