#include "TechTree/TechTree.h"
#include <iostream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace suboo {

/* Class Unit */
std::ostream& operator<<(std::ostream& os, const Unit& u) {
  /*auto print_abilities = [](const std::vector<AbilityId>& abilities) {
    std::string s = "";
    for (auto& a : abilities) {
      s += "[" + a.to_string();
      s += "]";
    }
    return s;
  };
  */
  os << "{" << std::endl
     << "\t type: " << sc2::UnitTypeToName(u.id) << std::endl
     << "\t name: " << u.name << std::endl
     << "\t mineral: " << u.mineral_cost << std::endl
     << "\t vespene: " << u.vespene_cost << std::endl
     << "\t food: " << u.food_cost << std::endl
     << "\t abilities: ";
  for (auto& a : u.abilities) {
    os << "[" << a.to_string() << "]";
  }
  os << std::endl << "}," << std::endl;
  return os;
}

/* Serialization function used for json output */



void to_json(json& j, const std::vector<AbilityId>& ab) { 
  for (auto& ability : ab) j.at("abilities").push_back((int)ability);
}

void from_json(const json& j, std::vector<AbilityId>& ab) {
  for (auto& ability : j) {
    ab.push_back((AbilityId)ability.get<int>());
  }
}

void to_json(json& j, const Unit& unit) {
  j["id"] = (int)unit.id;
  j["name"] = unit.name;
  j["mineral_cost"] = unit.mineral_cost;
  j["vespene_cost"] = unit.vespene_cost;
  j["food_cost"] = unit.food_cost;
  to_json(j, unit.abilities);
  j["requirement"] = (int)unit.requirement;
  j["action_status"] = unit.action_status;
}

void to_json(json& j, const std::unordered_map<int, Unit>& map) {
	for (auto & i : map) {
		json k;
		to_json(k, i.second);
		j.push_back(k);
	}
}


void from_json(const json& j, Unit& u) {
  u.id = j.at("id").get<int>();
  j.at("name").get_to(u.name);
  j.at("mineral_cost").get_to(u.mineral_cost);
  j.at("vespene_cost").get_to(u.vespene_cost);
  j.at("food_cost").get_to(u.food_cost);
  from_json(j, u.abilities);
  u.requirement = (UnitId)j.at("requirement").get<int>();
  j.at("action_status").get_to(u.action_status);
}

/* Class TechTree */
std::ostream& operator<<(std::ostream& os, const TechTree& t) {
  os << "{" << std::endl;
  os << "\t\"units\": [" << std::endl;
  for (auto& u : t.map) {
    os << u.second;
  }
  os << "\t]" << std::endl;
  os << "}";
  return os;
}

TechTree::TechTree() {}

TechTree& TechTree::getTechTree() {
  static TechTree tree;
  return tree;
}

void TechTree::addUnit(Unit u) { map.insert({u.id, u}); }
const Unit& TechTree::getUnit(UnitId id) const { return map.find(id)->second; }

std::string TechTree::serialize() {
	return "bjr";
}

}  // namespace suboo