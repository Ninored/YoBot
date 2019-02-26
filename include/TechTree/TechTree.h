#ifndef TECHTREE_H
#define TECHTREE_H

#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>
#include "sc2api/sc2_data.h"
#include "sc2api/sc2_unit.h"

namespace suboo {

using UnitId = sc2::UnitTypeID;    // UnitId using
using AbilityId = sc2::AbilityID;  // AbilityId using

/**
 * Unit Class
 */
class Unit {
 public:
  UnitId id;                         // Unit id
  std::string name;                  // Unit name
  int mineral_cost;                  // Unit mineral costs
  int vespene_cost;                  // Unit vespene costs
  int food_provided;                 // Unit food costs (can be positive)
  std::vector<AbilityId> abilities;  // Abilities of the unit

  UnitId requirement;  // Tech requirement

  int production_time;  // Unit creation time
  int travel_time;      // Unit travel time

  enum Status { BUSY, TRAVELLING, CONSUMING };
  Status action_status;  // Unit action behavior

  // Firends
  // friend std::ostream& operator<<(std::ostream& os, const Unit& u);
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
  std::string version;
  TechTree() {}

 public:
  static TechTree& getTechTree() {
    static TechTree tree;
    return tree;
  }

  const Unit& getUnit(UnitId id) const { return map.find(id)->second; }
  const std::string getVersion() const { return version; }
  const std::unordered_map<int, Unit>& getMap() const { return map; }

  void clear() { map.clear(); }
  void setVersion(std::string ver) { version = ver; }
  void addUnit(Unit u) { map.insert({u.id, u}); }

  void deserialize(std::string path) {
    std::ifstream file(path);
    if (file.fail()) {
      std::cerr << "Error while opening: " << path << std::endl;
      return;
    }
    nlohmann::json j;
    file >> j;

    this->setVersion(j["version"]);

    for (auto u : j["units"]) {
      Unit unit;
      unit.id = u.at("id").get<int>();
      u.at("name").get_to(unit.name);
      u.at("mineral_cost").get_to(unit.mineral_cost);
      u.at("vespene_cost").get_to(unit.vespene_cost);
      u.at("food_provided").get_to(unit.food_provided);
      unit.requirement = u.at("requirement").get<int>();
      u.at("action_status").get_to(unit.action_status);
      for (auto ab : u.at("abilities").get<std::vector<int>>())
        unit.abilities.push_back(ab);

      this->addUnit(unit);
    }
  }

  std::string serialize() {
    nlohmann::json root;

    // Version
    root["version"] = version;

    // Units
    for (auto entry : map) {
      auto u = entry.second;
      nlohmann::json unit;
      unit["id"] = (int)u.id;
      unit["name"] = u.name;
      unit["mineral_cost"] = u.mineral_cost;
      unit["vespene_cost"] = u.vespene_cost;
      unit["food_provided"] = u.food_provided;
      nlohmann::json abilities;
      for (auto ab : u.abilities) abilities.push_back((int)ab);
      unit["abilities"] = abilities;
      unit["requirement"] = (int)u.requirement;
      unit["action_status"] = u.action_status;
      root["units"] += unit;
    }
    return root.dump(2);
  }
};
}  // namespace suboo

#endif  // TECHTREE_H