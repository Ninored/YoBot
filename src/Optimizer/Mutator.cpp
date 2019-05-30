#include "Optimizer/Mutator.h"
#include "Optimizer/Opti.h"
#include "Optimizer/Prereq.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "Simulator/BuildOrder.h"
#include "Simulator/Simulator.h"
#include "Simulator/UnitTypes.h"

namespace suboo {

int setBest(BuildOrder& best, BuildOrder& candidate, const std::string& name) {
  int besttotal = best.getFinalState().getTimeStamp();
  int bestmin = best.getFinalState().getMinerals();
  int bestwait = 0;
  for (const auto& bi : best.getItems()) {
    bestwait += bi.get()->totalWait();
  }
  try {
    Simulator(candidate.getInitialState()).visite(candidate);
//    s.visite(candidate);

    auto total = candidate.getFinalState().getTimeStamp();
    auto bomin = candidate.getFinalState().getMinerals();
    int totalwait = 0;
    for (const auto& bi : candidate.getItems()) {
      totalwait += bi.get()->totalWait();
    }
    if (total < besttotal ||
        (total <= besttotal &&
         (totalwait < bestwait ||
          totalwait == bestwait &&
              (bomin > 1 + bestmin || ::abs(bomin - bestmin) < 1)))) {
      auto deltat = total - besttotal;
      auto deltamin = bomin - bestmin;
      auto deltawait = totalwait - bestwait;
      auto delta = deltat > 0 ? deltat : deltamin > 0 ? deltamin : deltawait;
      best = candidate;
      //			std::cout << name << " feasible in " << total;
      //			std::cout << " mins=" << bomin;
      //			std::cout << " totalwait=" << totalwait;
      //		    std::cout << " (new best)" << std::endl;
      return delta;
    }
  } catch (std::exception exc) {
  }
  return 0;
}

std::pair<int, BuildOrder> findBest(const BuildOrder& base,
                                    std::vector<BuildOrder>& candidates,
                                    std::vector<std::string> candDesc) {
  float best = base.getFinalState().getTimeStamp();
  float bestmin = base.getFinalState().getMinerals();
  int bestwait = 0;
  for (const auto& bi : base.getItems()) {
    bestwait += bi.get()->totalWait();
  }
  int basewait = bestwait;
  int bestindex = -1;
  int index = 0;
  for (auto& bo : candidates) {
    try {
      Simulator s(bo.getInitialState());
      s.visite(bo);
      auto total = bo.getFinalState().getTimeStamp();
      auto bomin = bo.getFinalState().getMinerals();
      int totalwait = 0;
      for (auto& bi : bo.getItems()) {
        totalwait += bi.get()->totalWait();
      }
      //			std::cout << candDesc[index] << " feasible in "
      //<< total; 			std::cout << " mins=" << bomin;
      // std::cout << " totalwait=" << totalwait;

      if (total < best ||
          (total <= best &&
           (totalwait < bestwait ||
            totalwait == bestwait &&
                (bomin > 1 + bestmin || ::abs(bomin - bestmin) < 1)))) {
        bestindex = index;
        bestmin = bomin;
        best = total;
        bestwait = totalwait;
        //				std::cout << " (new best)";
      }
      //			std::cout << std::endl;

    } catch (std::exception exc) {
      //			std::cout << candDesc[index] << " unfeasible."
      //<< std::endl;
    }
    index++;
  }

  int delta = (base.getFinalState().getTimeStamp() - best);
  int deltamin = bestmin - base.getFinalState().getMinerals();
  int deltawait = bestwait - basewait;

  // FIXME BuildOrder Empty
  return {delta > 0 ? delta : deltamin > 0 ? deltamin : deltawait,
          bestindex == -1 ? BuildOrder() : candidates[bestindex]};
}

std::pair<int, BuildOrder> Randomizer::improve(const BuildOrder& base,
                                               int depth) {
  BuildOrder best = base;
  for (int i = 0; i < 1000; i++) {
    auto copy = best;
    auto& deq = copy.getItems();
    std::shuffle(deq.begin(), deq.end(), e);
		EnforcePrereqBySwap(copy).visite(copy);
//    enforcePrereqBySwap(copy);
		// enforcePrereqBySwap devrait régler les prereq donc le build devrait etre assuré d'etre bon
		// Donc le try catch devrait pas etre présent 
    try {
      Simulator s(copy.getInitialState());
      s.visite(copy);
      auto timefin = copy.getFinalState().getTimeStamp();
      auto timebest = best.getFinalState().getTimeStamp();
      if (timefin < timebest ||
          timefin == timebest && (best.getFinalState().getMinerals() -
                                  copy.getFinalState().getMinerals()) > 0) {
        best = copy;
      }
    } catch (std::exception exc) {
    }
  }
  int delta = (base.getFinalState().getTimeStamp() -
                best.getFinalState().getTimeStamp());
  int deltamin =
      (base.getFinalState().getMinerals() - best.getFinalState().getMinerals());
  return {delta > 0 ? delta : deltamin, best};
}

std::pair<int, BuildOrder> LeftShifter::improve(const BuildOrder& base,
                                                int depth) {
  auto& tech = TechTree::getTechTree();
  std::vector<BuildOrder> candidates;
  std::vector<std::string> candindexes;
  candidates.reserve(20);
  // find any adjacent events with same timing
  auto& items = base.getItems();
  auto best = base;
  //  GameState current(tech.getInitialUnits(), tech.getInitialMinerals(),
  //  tech.getInitialVespene());
  GameState current = base.getInitialState();
  for (int i = 0, e = items.size(); i < e; i++) {
    auto bi = items[i].get();
    int skip = 0;
    for (int j = i + 1; j < e; j++) {
      auto bj = items[j].get();
      if (!(bj == bi)) {
        bool biprecedesbj = true;
        if (bi->getName().compare("BUILD") == 0 &&
            bj->getName().compare("BUILD") == 0) {
          auto& ui = tech.getUnit(((BIABuild*)bi)->getTarget());
          auto& uj = tech.getUnit(((BIABuild*)bj)->getTarget());
          if (uj.prereq != ui.id &&
              (uj.builder != ui.id ||
               uj.builder == sc2::UNIT_TYPEID::PROTOSS_PROBE ||
               current.hasFinishedUnit(uj.builder))) {
            biprecedesbj = false;
          }
        }
        if (bj->getName().compare("TRANSFER_VESPENE") &&
            bi->getName().compare("BUILD") &&
            ((BIABuild*)bi)->getTarget() ==
                sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) {
        } else {
          if (bi->getName().compare("BUILD") == 0 &&
              bj->getName().compare("BUILD") != 0) {
            biprecedesbj = false;
          }
        }
        if (bj->getTime() == bi->getTime() || !biprecedesbj) {
          BuildOrder bo = base;
          // std::stringstream sstr;
          // sstr << "swapping (" << i << " :"; bo.getItems()[i].print(sstr);
          // sstr << ") and (" << j << " :"; bo.getItems()[j].print(sstr);
          // sstr << ")" ;
          bo.swapItems(i, j);

	        EnforcePrereqBySwap(bo).visite(bo);
        /*  if (!enforcePrereqBySwap(bo)) {
            std::cout << "problem with swap prereq rule " << std::endl;
          }*/
          candidates.push_back(bo);
          // candindexes.push_back(sstr.str());
        }
        if (!(j < e - 1 && items[j + 1] == items[j])) {
          break;
        }
      } else {
        // skip++;
      }
    }
    if (bi->getName().compare("BUILD") == 0) {
      current.addUnit(((BIABuild*)bi)->getTarget());
    }
    i += skip;
  }
  return findBest(base, candidates, candindexes);
}

std::pair<int, BuildOrder> AddVespeneGatherer::improve(const BuildOrder& base,
                                                       int depth) {
  int nexi = base.getFinalState().countUnit(sc2::UNIT_TYPEID::PROTOSS_NEXUS);
  int ass =
      base.getFinalState().countUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
  if (2 * nexi > ass) {
    BuildOrder candidate = base;
    candidate.addItemFront(new BIAMineVespene());
    candidate.addItemFront(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
    try {
      Simulator s(candidate.getInitialState());
      s.visite(candidate);
      int delta = base.getFinalState().getTimeStamp() -
                  candidate.getFinalState().getTimeStamp();
      if (delta > 0) {
        return {delta, candidate};
      } else {
        if (depth > 0) {
          candidate = improveBO(candidate, std::min(depth - 1, 1));
          try {
            Simulator s(candidate.getInitialState());
            s.visite(candidate);
            // auto best = base;
            // if (setBest(base,candidate))
            std::vector<BuildOrder> candidates;
            candidates.push_back(candidate);
            return findBest(base, candidates, {"Add Gas forceful"});
          } catch (std::exception exc) {
          }
        }
      }
    } catch (std::exception exc) {
    }
  }
  return std::pair<int, BuildOrder>(0, BuildOrder());
}

std::pair<int, BuildOrder> AddMineralGatherer::improve(const BuildOrder& base,
                                                       int depth) {
  using UnitId = sc2::UNIT_TYPEID;
  std::vector<BuildOrder> candidates;
  candidates.reserve(base.getItems().size());
  std::vector<std::string> candnames;

  bool needpylon = false;
  if (base.getFinalState().getAvailableSupply() == 0) {
    needpylon = true;
  }
  auto& tech = TechTree::getTechTree();
  // int available = tech.getInitial().getAvailableSupply();
  int available = base.getInitialState().getAvailableSupply();
  if (base.getFinalState().probesToSaturation() > 0) {
    for (int i = 0, e = base.getItems().size(); i < e; i++) {
      if (available >= 1) {
        BuildOrder candidate = base;
        candidate.insertItem(sc2::UNIT_TYPEID::PROTOSS_PROBE, i);
        if (needpylon) {
          if (i <= 3) {
            candidate.insertItem(sc2::UNIT_TYPEID::PROTOSS_PYLON, 0);
          } else {
            candidate.insertItem(sc2::UNIT_TYPEID::PROTOSS_PYLON, i - 3);
          }
        }
        EnforcePrereqBySwap(candidate).visite(candidate);
/*        if (!EnforcePrereqBySwap(candidate).visite(candidate)) {
          std::cout << "problem with swap prereq rule " << std::endl;
        }*/
        candidates.push_back(candidate);
        // candnames.push_back("Add Probe at index " + std::to_string(i));

        // find the next pylons and bring them forward one
        for (int j = i + 2; j < e; j++) {
          auto act = candidate.getItems()[j].get();
          if (act->getName().compare("BUILD") == 0 &&
              ((BIABuild*)act)->getTarget() ==
                  sc2::UNIT_TYPEID::PROTOSS_PYLON) {
            candidate.swapItems(j, j - 1);
            candidates.push_back(candidate);
            // candnames.push_back("Add Probe at index " + std::to_string(i) +
            // " and shift pylon at index " + std::to_string(j));
          }
        }
      }
      auto& bi = base.getItems()[i];
      available += tech.getUnit(bi.get()->cast<BIABuild>()->getTarget()).food_provided;
    }
  }
  return findBest(base, candidates, candnames);
}

std::pair<int, BuildOrder> AddProduction::improve(const BuildOrder& base,
                                                  int depth) {
  auto& tech = TechTree::getTechTree();
  std::unordered_map<int, int> used;
  for (auto& bi : base.getItems()) {
    if (bi->getName().compare("BUILD") == 0) {
      auto& u = tech.getUnit(bi->cast<BIABuild>()->getTarget());
      if (u.builder != sc2::UNIT_TYPEID::INVALID &&
          !sc2util::IsWorkerType(u.builder)) {
        auto it = used.find(u.builder);
        if (it == used.end()) {
          used[u.builder] = 1;
        } else {
          it->second++;
        }
      }
    }
  }
  std::vector<BuildOrder> candidates;
  candidates.reserve(base.getItems().size());
  std::vector<std::string> candnames;
  for (auto& pair : used) {
    if (pair.second > base.getFinalState().countUnit(pair.first)) {
      auto& builder = tech.getUnit(pair.first);
      // try to stutter
      int index = 0;
      bool ok = false;
      for (auto& bi : base.getItems()) {
        if (ok || bi.get()->getName().compare("BUILD") &&
                      bi.get()->cast<BIABuild>()->getTarget() == pair.first) {
          auto candidate = base;
          candidate.insertItem(pair.first, index);
          candidates.push_back(candidate);
          // candnames.push_back("Add production " + builder.name + " at step
          // "
          // + std::to_string(index));
          ok = true;
        }
        index++;
      }
    }
  }
  return findBest(base, candidates, candnames);
}

std::pair<int, BuildOrder> NoWaitShifter::improve(const BuildOrder& base,
                                                  int depth) {
  auto& tech = TechTree::getTechTree();
  std::vector<BuildOrder> candidates;
  std::vector<std::string> candindexes;
  candidates.reserve(20);
  // find any event with zero wait : left shift it
  auto& items = base.getItems();
  // GameState current = tech.getInitial();
  GameState current(base.getInitialState());
  for (int i = 1, e = items.size(); i < e; i++) {
    auto bi = items[i].get();
    if (bi == items[i - 1].get()) {
      continue;
    }
    if (bi->getName().compare("TRANSFER_VESPENE") == 0 &&
        items[i - 1].get()->cast<BIABuild>()->getTarget() ==
            sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR) {
      // causes build to be broken
      continue;
    }
    if (bi->totalWait() < 0) {
      BuildOrder bo = base;
      // std::stringstream sstr;
      // sstr << "swapping (" << i-1 << " :"; bo.getItems()[i-1].print(sstr);
      // sstr << ") and (" << i << " :"; bo.getItems()[i].print(sstr);
      // sstr << ")";
      bo.swapItems(i, i - 1);

      candidates.push_back(bo);
      // candindexes.push_back(sstr.str());

      if (bo.getItems()[i].get()->totalWait() < 0) {
        for (int j = i - 2; j >= 0; j--) {
          // chains of nowait, try swapping the blocking thing with any of the
          // non blocked events
          if (bo.getItems()[j].get()->totalWait() < 0) {
            continue;
          } else {
            BuildOrder bo = base;
            // std::stringstream sstr;
            // sstr << "swapping (" << j << " :";
            // bo.getItems()[j].print(sstr); sstr << ") and (" << i << " :";
            // bo.getItems()[i].print(sstr); sstr << ")";
            bo.swapItems(i, j);

            candidates.push_back(bo);
            // candindexes.push_back(sstr.str());
            break;
          }
        }
      }
    }
  }
  return findBest(base, candidates, candindexes);
}
std::pair<int, BuildOrder> AddMineralGathererStack::improve(
    const BuildOrder& base, int depth) {
  if (base.getFinalState().probesToSaturation() > 0) {
    auto copy = base;
    int i = 0, tobuild = base.getFinalState().probesToSaturation();
    for (; i < tobuild && i < 3; i++) {
      copy.addItemFront(sc2::UNIT_TYPEID::PROTOSS_PROBE);
    }
    copy = EnforcePrereq().visite(copy);
//    copy = ep.visite(copy);
    try {
      Simulator s(copy.getInitialState());
      s.visite(copy);
      if (depth > 0) copy = improveBO(copy, std::min(depth - 1, 1));
      std::vector<BuildOrder> candidates;
      candidates.push_back(copy);
      return findBest(base, candidates, {"optimizedSaturation"});
    } catch (std::exception exc) {
    }
  }
  return std::pair<int, BuildOrder>(0, BuildOrder());
}  // namespace suboo
std::pair<int, BuildOrder> AddProductionForceful::improve(
    const BuildOrder& base, int depth) {
  auto& tech = TechTree::getTechTree();

  std::map<UnitId, int> waitFor;
  for (auto& bi : base.getItems()) {
    if (bi.get()->getName().compare("BUILD") == 0) {
      if (bi.get()->timeFreeUnit > 0) {
        auto& unit = tech.getUnit(bi.get()->cast<BIABuild>()->getTarget());
        if (unit.builder != sc2::UNIT_TYPEID::INVALID) {
          waitFor[unit.builder] += bi.get()->timeFreeUnit;
        }
      }
    }
  }
  std::vector<std::pair<UnitId, int> > pairs(waitFor.begin(), waitFor.end());
  std::sort(pairs.begin(), pairs.end(),
            [](auto& a, auto& b) { return a.second > b.second; });
  std::vector<BuildOrder> candidates;
  std::vector<std::string> candindexes;
  for (auto& pair : pairs) {
    if (pair.second >= 30) {
      auto copy = base;
      copy.addItem(pair.first);
      copy = EnforcePrereq().visite(copy);
      try {
        Simulator s(copy.getInitialState());
        s.visite(copy);
        if (depth > 0)
          copy = improveBO(copy, std::min(depth - 1, 3));

        candidates.push_back(copy);
        // candindexes.push_back("Forcefully add production :" +
        // tech.getUnitById(pair.first).name);
      } catch (std::exception exc) {
      }
    }
  }
  return findBest(base, candidates, candindexes);
}
std::pair<int, BuildOrder> RemoveExtra::improve(const BuildOrder& base,
                                                int depth) {
  std::vector<BuildOrder> candidates;
  std::vector<std::string> candindexes;
  int foodPer = TechTree::getTechTree()
                    .getUnit(sc2::UNIT_TYPEID::PROTOSS_PYLON)
                    .food_provided;
  if (base.getFinalState().getAvailableSupply() > foodPer) {
    auto bo = base;
    // find last pylon
    for (auto it = bo.getItems().begin() + (bo.getItems().size() - 1),
              e = bo.getItems().begin();
         it != e; --it) {
      if (it->get()->getName().compare("BUILD") &&
          it->get()->cast<BIABuild>()->getTarget() == sc2::UNIT_TYPEID::PROTOSS_PYLON) {
        bo.getItems().erase(it);
        break;
      }
    }
  }
  return std::pair<int, BuildOrder>();
}
}  // namespace suboo
