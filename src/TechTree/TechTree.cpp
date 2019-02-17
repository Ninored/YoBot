#include "TechTree/TechTree.h"
#include <iostream>

namespace suboo {

/* Class Unit */
std::ostream& operator<<(std::ostream& os, const Unit& u) {
  os << u.name << std::endl;
  return os;
}

/* Class TechTree */
TechTree::TechTree(){}

TechTree& TechTree::getTechTree() {
  static TechTree tree;
  return tree;
}

}