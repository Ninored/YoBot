#include "TechTree/TechTree.hpp"
#include <iostream>

namespace suboo {
  TechTree::TechTree() {}
  TechTree & TechTree::getTechTree() {
    static TechTree tree;
    return tree;
  }

  void TechTree::Test() { std::cout << "TEST TechTree" << std::endl;}

} // namespace suboo