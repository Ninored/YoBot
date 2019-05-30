#include "Simulator/GameState.h"
#include <algorithm>
#include "Simulator/UnitTypes.h"
#include "TechTree/TechTree.h"

namespace suboo {

GameState::GameState()
    : minerals(TechTree::getTechTree().getInitialMinerals()),
      vespenes(TechTree::getTechTree().getInitialVespene()),
      mps(0),
      vps(0),
      timestamp(0) {}

GameState::GameState(const std::vector<UnitInstance>& units, int m, int v)
    : minerals(m), vespenes(v), mps(0), vps(0), timestamp(0) {
  for (auto u : units) {
    addUnit(u);
  }
  calculateMps();
  calculateVps();
  calculateSupply();
}

// Calculate
void GameState::calculateMps() {
  mps = 0;
  for (auto u : freeUnits) {
    if (u.type == sc2::UNIT_TYPEID::PROTOSS_PROBE &&
        u.state == u.MINING_MINERALS)
      mps += 0.896f;
  }
}
void GameState::calculateVps() {
  vps = 0;
  for (auto u : busyUnits) {
    if (u.type == sc2::UNIT_TYPEID::PROTOSS_PROBE &&
        u.state == u.MINING_VESPENE)
      vps += 0.649f;
  }
}
void GameState::calculateSupply() {
  supply = 0;
  auto& tech = TechTree::getTechTree();

  for (auto& u : freeUnits) {
    if (tech.getUnit(u.type).food_provided < 0) {
      supply += tech.getUnit(u.type).food_provided;
    } else if (u.state != u.BUILDING) {
      supply += tech.getUnit(u.type).food_provided;
    }
  }
  for (auto& u : busyUnits) {
    if (tech.getUnit(u.type).food_provided < 0) {
      supply += tech.getUnit(u.type).food_provided;
    } else if (u.state != u.BUILDING) {
      supply += tech.getUnit(u.type).food_provided;
    }
  }
}
// Mutate State
/* By default add the unit to free list */
bool GameState::addUnit(UnitId unit) { return addUnit(UnitInstance(unit)); }

bool GameState::addUnit(const UnitInstance& unit) {
  if (unit.state == unit.MINING_MINERALS || unit.state == unit.MINING_VESPENE ||
      sc2util::IsWorkerType(unit.type)) {
    freeUnits.push_back(unit);
    calculateMps();
    calculateVps();
    calculateSupply();
    return true;
  }
  if (unit.state == unit.FREE) {
    freeUnits.push_back(unit);
  } else {
    busyUnits.push_back(unit);
    calculateSupply();
    return true;
  }

  return true;
}

// Step function
void GameState::step(int sec) {
  for (int i = 0; i < sec; ++i) {
    for (auto it = busyUnits.begin(); it != busyUnits.end(); /* NA */) {
      /* Add energy FIXME: on all units not only busy*/
      if (it->energy < 200) {
        it->energy += 0.7875;
      }
      if (it->time_to_free > 0) {
        /*Check time with chrono*/
        if (it->time_with_chronoboost > 0) {
          it->time_with_chronoboost--;
          it->time_to_free -= 1.5;
        } else {
          it->time_to_free--;
        }

        if (it->time_to_free <= 0) {
          if (!sc2util::IsWorkerType(it->type))
            it->state = it->FREE;
          else
            it->state = it->MINING_MINERALS;
          setFree(it);
        } else {
          ++it;
        }
      }
    }

    minerals += getMps();
    vespenes += getVps();
    timestamp += 1;
  }
}

// GET
int GameState::getAvailableSupply() const { return supply; }
int GameState::getMaxSupply() const { return supply; }
int GameState::getTimeStamp() const { return timestamp; }
float& GameState::getMinerals() { return minerals; }
float GameState::getMinerals() const { return minerals; }
float GameState::getMps() const { return mps; }
float& GameState::getVespene() { return vespenes; }
float GameState::getVespene() const { return vespenes; }
float GameState::getVps() const { return vps; }
std::vector<UnitInstance>& GameState::getFreeUnits() { return freeUnits; }
std::vector<UnitInstance>& GameState::getBusyUnits() { return busyUnits; }

// SET
void GameState::setFree(std::vector<UnitInstance>::iterator& it) {
  freeUnits.push_back(*it);
  it = busyUnits.erase(it);
}
void GameState::setBusy(std::vector<UnitInstance>::iterator& it) {
  busyUnits.push_back(*it);
  it = freeUnits.erase(it);
}

// Mutate unit
/* Add alternative to assign prob to Minerales */
bool GameState::assignProbe(UnitInstance::UnitState state) {
  int max = 3;
  for (auto it = freeUnits.begin(); it != freeUnits.end(); /* NA */) {
    if (max <= 0) break;
    auto& u = *it;
    if (sc2util::IsWorkerType(u.type) && u.state == u.FREE ||
        u.state == u.MINING_MINERALS) {
      u.state = u.MINING_VESPENE;
      setBusy(it);
      max--;
      continue;
    }
    ++it;
  }

  calculateVps();
  calculateMps();
  return max <= 0;
}

bool GameState::assignFreeUnit(UnitId type, UnitInstance::UnitState state,
                               int time) {
  if (state == UnitInstance::BUSY) {
    auto it = std::find_if(freeUnits.begin(), freeUnits.end(),
                           [type](auto u) { return u.type == type; });
    if (it == freeUnits.end()) return false;

    it->time_to_free = time;
    it->state = state;
    setBusy(it);
  }
  calculateMps();
  calculateVps();
  return true;
}

// Has
bool GameState::hasFreeUnit(UnitId unit) const {
  return std::any_of(freeUnits.begin(), freeUnits.end(),
                     [unit](auto& u) { return u.type == unit; });
}

bool GameState::hasFinishedUnit(UnitId unit) const {
  return std::any_of(busyUnits.begin(), busyUnits.end(),
                     [unit](auto& u) {
                       return (u.type == unit && u.state != u.BUILDING);
                     }) ||
         std::any_of(freeUnits.begin(), freeUnits.end(), [unit](auto& u) {
           return (u.type == unit && u.state != u.BUILDING);
         });
}

// Wait function
bool GameState::waitforUnitCompletion(UnitId id) {
  auto it = std::find_if(busyUnits.begin(), busyUnits.end(), [id](auto& u) {
    return u.type == id && u.state == u.BUILDING;
  });
  if (it == busyUnits.end()) return false;

  std::cout << "[waitforUnitCompletion] wait for ( " << (int)id
            << "): " << it->time_to_free << std::endl;
  step(it->time_to_free);
  return true;
}
bool GameState::waitforUnitFree(UnitId id) {
  /* Search builder */
	// FIXME: filtrer les busyUnits et ensuite trouver le minimume
  auto it = std::min_element(
      std::begin(busyUnits), std::end(busyUnits),
      [id](const UnitInstance& a, const UnitInstance& b) {
        return a.time_to_free - (a.time_with_chronoboost * 1.5) <
							 b.time_to_free - (b.time_with_chronoboost * 1.5) &&
               a.type == id && b.type == id;
      });

  if (it == busyUnits.end()) return false;

  std::cout << "[waitforUnitFree] wait for (" << (int)id
            << "): " << it->time_to_free << std::endl;
  // FIXME: we dont take in count the chronoboost.
  step(it->time_to_free - (it->time_with_chronoboost * 1.5));

  return true;
}

bool GameState::waitforFreeSupply(int nedded) {
  int cur = getAvailableSupply();

  if (cur >= nedded) {
    return true;
  }

  auto it = std::find_if(busyUnits.begin(), busyUnits.end(), [](auto& u) {
    return u.type == sc2::UNIT_TYPEID::PROTOSS_PYLON ||
           u.type == sc2::UNIT_TYPEID::PROTOSS_NEXUS && u.state != u.FREE;
  });
  if (it == busyUnits.end()) {
    return false;
  }
  int index = 0;
  int best = -1;
  for (auto& u : busyUnits) {
    if (u.state == u.BUILDING && (u.type == sc2::UNIT_TYPEID::PROTOSS_PYLON ||
                                  u.type == sc2::UNIT_TYPEID::PROTOSS_NEXUS)) {
      if (best == -1 || busyUnits[best].time_to_free > u.time_to_free) {
        best = index;
      }
    }
    index++;
  }
  if (best == -1) {
    return false;
  } else {
    // std::cout << "Waited for " <<
    // TechTree::getTechTree().getUnitById(units[best].type).name << " to be
    // provide food for " << units[best].time_to_free << "s." << std::endl;
    step(busyUnits[best].time_to_free);
    return true;
  }
  return true;
}

bool GameState::waitforAllUnitFree() {
  // FIXME:

  int time_to_free = 0;
  for (auto u : busyUnits)
    if (time_to_free < u.time_to_free) time_to_free = u.time_to_free;

  step(time_to_free);
  return true;
}
std::pair<int, int> GameState::waitForRessources(int min, int ves) {
  std::pair<int, int> res(0, 0);

  // wait minerals
  if (min > minerals) {
    int msecs = ((min - minerals) / mps) + 1;
    step(msecs);
    res.first = msecs;
  }

  // wait vespene
  if (ves > vespenes) {
    int vsecs = ((ves - vespenes) / vps) + 1;
    step(vsecs);
    res.second = vsecs;
  }

  std::cout << "[waitForRessources] min: " << res.first
            << " ves: " << res.second << ": " << std::endl;
  return res;
}

// Count
int GameState::countUnit(UnitId id) const {
  return std::count_if(freeUnits.begin(), freeUnits.end(),
                       [id](auto& u) { return u.type == id; }) +
         std::count_if(busyUnits.begin(), busyUnits.end(),
                       [id](auto& u) { return u.type == id; }) +
         std::count_if(attackUnits.begin(), attackUnits.end(),
                       [id](auto& u) { return u.type == id; });
}
int GameState::countFreeUnit(UnitId id) const {
  return std::count_if(freeUnits.begin(), freeUnits.end(), [id](auto& u) {
    return u.type == id && u.state == UnitInstance::FREE;
  });
}

// Tools
int GameState::probesToSaturation() const {
  int nexi = countUnit(sc2::UNIT_TYPEID::PROTOSS_NEXUS);
  int ass = countUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
  int prob = countUnit(sc2::UNIT_TYPEID::PROTOSS_PROBE);
  return (20 * nexi + 3 * ass) - prob;
}

std::ostream& operator<<(std::ostream& os, const GameState& state) {
  os << "[GS:" << state.timestamp << "]" << std::endl
     << "\t[Supply]: " << state.supply << std::endl
     << "\t[Minerals]: " << state.minerals << ", [Vespene]: " << state.vespenes
     << ", mps:" << state.mps << ", vps:" << state.vps << ")" << std::endl;

  os << "\t[freeUnits]: (";
  for (auto u : state.freeUnits) {
    os << u.type << ", ";
  }
  os << " )" << std::endl;

  os << "\t[busyUnits]: (";
  for (auto u : state.busyUnits) {
    os << u.type << ", ";
  }
  os << " )" << std::endl;
  return os;
}

}  // namespace suboo
