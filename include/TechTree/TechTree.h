#ifndef TECHTREE_H
#define TECHTREE_H

#include <fstream>
#include <iostream>
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
  UnitId id;                      // Unit id
  std::string name;               // Unit name
  int mineral_cost;               // Unit mineral costs
  int vespene_cost;               // Unit vespene costs
  int food_provided;              // Unit food costs (can be positive)
  std::set<AbilityId> abilities;  // Abilities of the unit

  UnitId prereq;   // Tech requirement
  UnitId builder;  // Builder Id

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
  enum UnitState { BUILDING, BUSY, MINING_MINERALS, MINING_VESPENE, FREE };
  UnitId type;
  UnitState state;
  int time_to_free;

  UnitInstance(UnitId id) : type(id), state(MINING_MINERALS), time_to_free(0){};
  UnitInstance(UnitId id, UnitState state, int time)
      : type(id), state(state), time_to_free(time){};

  void print(std::ostream& out) const;
  std::string print_status() const {
    switch (state) {
      case suboo::UnitInstance::BUILDING: return "B";
      case suboo::UnitInstance::BUSY: return "C";
      case suboo::UnitInstance::MINING_MINERALS: return "M";
      case suboo::UnitInstance::MINING_VESPENE: return "V";
      case suboo::UnitInstance::FREE: return "F";
      default : return "-" ;
    }
  }
};

/**
 * Singleton class used to query information about units
 * It can be serialized and deserialized
 */
class TechTree {
  std::unordered_map<int, Unit> map;

  std::string version;
  int initial_minerales;
  int initial_vespene;
  std::vector<UnitInstance> initial_unitinstances;
  TechTree() {}

 public:
  static TechTree& getTechTree() {
    static TechTree tree;
    return tree;
  }

  const Unit& getUnit(UnitId id) const {
    auto& u = map.find(id);
    if (u == map.end())
      throw std::out_of_range("Cannot find unit id: " + (int)id);

    return map.find(id)->second;
  }
  const std::string getVersion() const { return version; }
  const std::unordered_map<int, Unit>& getMap() const { return map; }
  const std::vector<UnitInstance> getInitialUnits() const {
    return initial_unitinstances;
  }
  const int getInitialMinerals() const { return initial_minerales; }
  const int getInitialVespene() const { return initial_vespene; }

  void clear() { map.clear(); }
  void setVersion(std::string ver) { version = ver; }
  void addUnit(Unit u) { map.insert({u.id, u}); }

  void setInitialState(std::vector<UnitInstance>& units, int i_minerales,
                       int i_vespene) {
    initial_unitinstances.insert(std::begin(initial_unitinstances),
                                 std::begin(units), std::end(units));

    initial_minerales = i_minerales;
    initial_vespene = i_vespene;
  }

  void deserialize(std::string path) {
    std::ifstream file(path);
    if (file.fail()) {
      std::cerr << "Error while opening: " << path << std::endl;
      return;
    }
    nlohmann::json j;
    file >> j;

    j.at("version").get_to(version);
    j.at("initial_minerales").get_to(initial_minerales);
    j.at("initial_vespene").get_to(initial_vespene);

    nlohmann::json i_units = j.at("initial_units");
    if (!i_units.is_null())
      for (auto u : i_units.get<std::vector<int>>())
        initial_unitinstances.emplace_back(UnitInstance(u));

    for (auto u : j["units"]) {
      Unit unit;
      unit.id = u.at("id").get<int>();
      u.at("name").get_to(unit.name);
      u.at("mineral_cost").get_to(unit.mineral_cost);
      u.at("vespene_cost").get_to(unit.vespene_cost);
      u.at("food_provided").get_to(unit.food_provided);
      u.at("production_time").get_to(unit.production_time);
      u.at("travel_time").get_to(unit.travel_time);
      unit.prereq = u.at("requirement").get<int>();
      u.at("action_status").get_to(unit.action_status);
      if (!u.at("abilities").is_null())
        for (auto ab : u.at("abilities").get<std::vector<int>>())
          unit.abilities.insert(ab);

      this->addUnit(unit);
    }
  }

  std::string serialize() {
    nlohmann::json root;

    // Version
    root["version"] = version;
    root["initial_minerales"] = initial_minerales;
    root["initial_vespene"] = initial_vespene;

    nlohmann::json init_units;
    for (auto& units : initial_unitinstances)
      init_units.push_back((int)units.type);
    root["initial_units"] = init_units;

    // Units
    for (auto entry : map) {
      auto u = entry.second;
      nlohmann::json unit;
      unit["id"] = (int)u.id;
      unit["name"] = u.name;
      unit["mineral_cost"] = u.mineral_cost;
      unit["vespene_cost"] = u.vespene_cost;
      unit["food_provided"] = u.food_provided;
      unit["production_time"] = u.production_time;
      unit["travel_time"] = u.travel_time;
      nlohmann::json abilities;
      for (auto ab : u.abilities) abilities.push_back((int)ab);
      unit["abilities"] = abilities;
      unit["builder"] = (int)u.builder;
      unit["requirement"] = (int)u.prereq;
      unit["action_status"] = u.action_status;
      root["units"] += unit;
    }
    return root.dump(2);
  }
};
}  // namespace suboo

#endif  // TECHTREE_H