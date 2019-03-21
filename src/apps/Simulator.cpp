#include <iostream>
#include "Simulator/BuildOrderGoal.h"
#include "Simulator/BuildOrder.h"
#include "TechTree/TechTree.h"

using namespace suboo;

int main(int argc, char** argv) {
  // Loading techtree
  TechTree::getTechTree().deserialize("techtree_.json");

  BuildOrder bo;
  BuildOrderGoal goal;
  goal.addUnit(sc2::UNIT_TYPEID::PROTOSS_IMMORTAL, 4);
  std::cout << goal << std::endl;

  bo.addItem(BuildItem(BuildItem::BUILD, sc2::UNIT_TYPEID::INVALID));
  bo.addItem(BuildItem(BuildItem::BUILD, sc2::UNIT_TYPEID::INVALID));
  bo.addItem(BuildItem(BuildItem::BUILD, sc2::UNIT_TYPEID::INVALID));
  bo.addItem(BuildItem(BuildItem::BUILD, sc2::UNIT_TYPEID::INVALID));
  std::cout << bo << std::endl;

  return 0;
}