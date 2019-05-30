#include "Optimizer/Prereq.h"

namespace suboo {

/* Enforce Prereq */

EnforcePrereq::EnforcePrereq() : tech(TechTree::getTechTree()), vesp(0) {}
EnforcePrereq::EnforcePrereq(BuildOrder& b) : out_bo(b.getInitialState()), tech(TechTree::getTechTree()), vesp(0) {}
void EnforcePrereq::visite(BIABuild& e) {
  auto target = e.getTarget();
  auto& unit = tech.getUnit(target);
  // unit prereq
  std::vector<std::unique_ptr<BIA>> pre;
  addPreReq(pre, state, seen, unit.id, tech);
  for (auto& id : pre) {
    out_bo.addItem(id);
  }
}
void EnforcePrereq::visite(BIAMineVespene& e) {
  if (state.countUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) >= vesp) {
    out_bo.addItem(e.clone());
    vesp++;
  }
}

void EnforcePrereq::visite(BIAWaitGoal& e) {
  if (state.countUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) >= vesp) {
    out_bo.addItem(e.clone());
    vesp++;
  }
}
void EnforcePrereq::visite(BIAChronoboost& e) {
  if (state.countUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) >= vesp) {
    out_bo.addItem(e.clone());
    vesp++;
  }
}
BuildOrder EnforcePrereq::visite(BuildOrder& bo) {
  state = bo.getInitialState();
  std::cout << "STATE STATE  " << std::endl << state << std::endl;
  for (auto& u : state.getFreeUnits()) {
    seen.insert(u.type);
  }
  for (auto& u : state.getBusyUnits()) {
    seen.insert(u.type);
  }
  for (auto& bia : bo.getItems()) bia.get()->accept(*this);
  return out_bo;
}

/* Enforce Prereq By Swap */

EnforcePrereqBySwap::EnforcePrereqBySwap(BuildOrder& b)
    : bo(b), tech(TechTree::getTechTree()), vesp(0), i(0) {}

void EnforcePrereqBySwap::visite(BIABuild& e) {
  auto target = e.getTarget();
  auto& unit = tech.getUnit(target);
  // unit prereq
  addPreReq(pre, state, seen, unit.id, tech);
  for (auto& p : pre) {
    int j, e;
    for (j = i, e = bo.getItems().size(); j < e; j++) {
      if (*bo.getItems()[j] == *p ||
          (*p == BIABuild(sc2::UNIT_TYPEID::PROTOSS_PYLON) &&
           *(bo.getItems()[j]) ==
               BIABuild(sc2::UNIT_TYPEID::PROTOSS_NEXUS))) {
        bo.removeItem(j);
        if (i == j) {
          i--;
        }
        break;
      }
    }
    if (j == e) {
      throw std::runtime_error("visite enforce by swap");
      // break;
      //      return false;
    }
    bo_pre.addItem(p);
    if (*p == BIABuild(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR)) {
      auto bbi = BIAMineVespene();
      for (j = std::max(0, i), e = bo.getItems().size(); j < e; j++) {
        if (bo.getItems()[j]->getName().compare("MINEVESPENE") == 0) {
          bo.removeItem(j);
          if (i == j) {
            i--;
          }
          break;
        }
      }
      if (j == e) {
        throw std::runtime_error("visite enforce by swap");
        // return;

        //  return false;
      }
      bo_pre.addItem(new BIAMineVespene());
    }
  }
}
void EnforcePrereqBySwap::visite(BIAMineVespene& e) {
  bo_pre.addItem(e.clone());
}
void EnforcePrereqBySwap::visite(BIAWaitGoal& e) { bo_pre.addItem(e.clone()); }
void EnforcePrereqBySwap::visite(BIAChronoboost& e) {
  bo_pre.addItem(e.clone());
}
void EnforcePrereqBySwap::visite(BuildOrder& bo) {
  state = bo.getInitialState();
  for (auto& u : state.getFreeUnits()) {
    seen.insert(u.type);
  }
  for (auto& u : state.getBusyUnits()) {
    seen.insert(u.type);
  }
  for (this->i = 0; this->i < bo.getItems().size(); this->i++) bo.getItems()[i]->accept(*this);

  bo = bo_pre;
}

void addPreReq(std::vector<std::unique_ptr<BIA>>& pre, GameState& state,
               std::unordered_set<int>& seen, UnitId target,
               const TechTree& tech) {
  auto& unit = tech.getUnit(target);
  if ((int)unit.prereq != 0) {
    if (!state.hasFinishedUnit(unit.prereq)) {
      addPreReq(pre, state, seen, unit.prereq, tech);
    }
  }
  if ((int)unit.builder != 0) {
    if (!state.hasFreeUnit(unit.builder)) {
      addPreReq(pre, state, seen, unit.builder, tech);
    }
  }
  // food
  auto soup = state.getAvailableSupply();
  if (unit.food_provided < 0 && soup < -unit.food_provided) {
    pre.push_back(
        std::unique_ptr<BIA>(new BIABuild(sc2::UNIT_TYPEID::PROTOSS_PYLON)));
    state.addUnit(sc2::UNIT_TYPEID::PROTOSS_PYLON);
  }
  // vespene
  if (unit.vespene_cost > 0 &&
      !state.hasFinishedUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR)) {
    pre.push_back(std::unique_ptr<BIA>(
        new BIABuild(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR)));
    state.addUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
    pre.push_back(std::unique_ptr<BIA>(new BIAMineVespene()));
  }
  pre.push_back(std::unique_ptr<BIA>(new BIABuild(unit.id)));
  state.addUnit(unit.id);
  seen.insert(unit.id);
}

/*
bool enforcePrereqBySwap(BuildOrder& bo) {
  auto& tech = TechTree::getTechTree();
  // enforce prerequisites
  GameState state = bo.getInitialState();
  BuildOrder bopre;
  std::unordered_set<UnitId> seen;
  for (auto& u : state.getFreeUnits()) {
    seen.insert(u.type);
  }
  for (auto& u : state.getBusyUnits()) {
    seen.insert(u.type);
  }
  int sz = bo.getItems().size();
  for (int i = 0; i < bo.getItems().size() && i >= 0; i++) {
    auto bi = bo.getItems()[i].get();
    if (bi->getName().compare("BUILD") == 0) {
      auto target = bi->cast<BIABuild>()->getTarget();
      auto& unit = tech.getUnit(target);
      // unit prereq
      std::vector<std::unique_ptr<BIA>> pre;
      addPreReq(pre, state, seen, unit.id, tech);

      for (auto& p : pre) {
        auto id = p.get();
        int j, e;
        for (j = i, e = bo.getItems().size(); j < e; j++) {
          if (bo.getItems()[j].get() == id ||
              (*id == BIABuild(sc2::UNIT_TYPEID::PROTOSS_PYLON) &&
               *(bo.getItems()[j].get()) ==
                   BIABuild(sc2::UNIT_TYPEID::PROTOSS_NEXUS))) {
            bo.removeItem(j);
            if (i == j) {
              i--;
            }
            break;
          }
        }
        if (j == e) {
          return false;
        }
        bopre.addItem(id);
        if (*id == BIABuild(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR)) {
          auto bbi = BIAMineVespene();
          for (j = std::max(0, i), e = bo.getItems().size(); j < e; j++) {
            if (bo.getItems()[j].get()->getName().compare("MINEVESPENE") == 0) {
              bo.removeItem(j);
              if (i == j) {
                i--;
              }
              break;
            }
          }
          if (j == e) {
            return false;
          }
          bopre.addItem(new BIAMineVespene());
        }
      }
    } else {
      bopre.addItem(bi);
    }
  }
  bo = bopre;
  return true;
}
*/

/*
BuildOrder enforcePrereq(const BuildOrder& bo) {
  auto& tech = TechTree::getTechTree();
  // enforce prerequisites
  GameState state = bo.getInitialState();
  BuildOrder bopre;
  std::unordered_set<UnitId> seen;
  for (auto& u : state.getFreeUnits()) {
    seen.insert(u.type);
  }
  for (auto& u : state.getBusyUnits()) {
    seen.insert(u.type);
  }
  int vesp = 0;
  for (auto& bia : bo.getItems()) {
    if (bia.get()->getName().compare("BUILD") == 0) {
      auto bi = (BIABuild*)bia.get();
      auto target = bi->getTarget();
      auto& unit = tech.getUnit(target);
      // unit prereq
      std::vector<std::unique_ptr<BIA>> pre;
      addPreReq(pre, state, seen, unit.id, tech);

      for (auto& id : pre) {
        bopre.addItem(id);
      }
    } else {
      if (state.countUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) >= vesp) {
        bopre.addItem(bia);
        vesp++;
      }
    }
  }
  return bopre;
}
*/
}  // namespace suboo