#include "TechTree/TechTree.h"
#include <iostream>

namespace suboo {

/* Class Unit */
std::ostream& operator<<(std::ostream& os, const Unit& u) {
  auto print_abilities = [](const std::vector<AbilityId>& abilities) {
    std::string s = "";
    for (auto& a : abilities) {
      s += "[" + a.to_string();
      s += "]";
    }
    return s;
  };
  os << "{" << std::endl
     << "\t type: " << u.id << std::endl
     << "\t name: " << u.name << std::endl
     << "\t mineral: " << u.mineral_cost << std::endl
     << "\t vespen: " << u.vespene_cost << std::endl
     << "\t food: " << u.food_cost << std::endl
     << "\t abilities: ";
  for (auto a : u.abilities) {
    os << "[" << a.to_string() << "]";
  }
  os << std::endl
     << "}" << std::endl;
  return os;
}

std::size_t Unit::operator()(const Unit& k) const { return std::hash<int>()(id); }
bool Unit::operator==(const Unit& other) const { return id == other.id; }

/* Class TechTree */

std::ostream& operator<<(std::ostream& os, const TechTree& t) { 
  for (auto& u : t.map) {
    os << u.second;
  }
  return os;
}

TechTree::TechTree(){}

TechTree& TechTree::getTechTree() {
  static TechTree tree;
  return tree;
}

void TechTree::addUnit(Unit u) { map.insert({u.id, u}); } 
const Unit& TechTree::getUnit(UnitId id) const { return map.find(id)->second; }
}