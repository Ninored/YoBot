#include "BuildOrder/GameState.h"
#include "BuildOrder/UnitTypes.h"

using namespace suboo;

inline int getFood(const UnitInstance &u, const TechTree &tech) {
  auto &unit = tech.getUnit(u.type);
  if (unit.food_provided < 0) {
    return unit.food_provided;
  } else if (u.state != u.BUILDING) {
    return unit.food_provided;
  }
  return 0;
}

GameState::GameState(const std::vector<UnitInstance> &units, int minerals,
          int vespene)
    : freeUnits(units),
      minerals(minerals),
      mps(-1.0),
      vespene(vespene),
      vps(-1.0),
      supply(-1),
      timestamp(0) {}

// Step
void GameState::stepForward(int secs) {
  for (int i = 0; i < secs; i++) {
    bool changed = false;
    for (auto it = busyUnits.begin(); it != busyUnits.end(); /*NOP*/) {
      auto &u = *it;
      bool erased = false;
      if (u.time_to_free > 0) {
        u.time_to_free--;
        if (u.time_to_free == 0) {
          if (!sc2util::IsWorkerType(u.type)) {
            u.state = u.FREE;
            auto &unit = TechTree::getTechTree().getUnit(u.type);
            addUnit(u);
            erased = true;
          } else {
            // default for workers is to mine
            u.state = u.MINING_MINERALS;
            addUnit(u);
            erased = true;
          }
          changed = true;
        }
      }
      if (erased) {
        it = busyUnits.erase(it);
      } else {
        ++it;
      }
    }
    if (changed) {
      mps = -1.0f;
      vps = -1.0f;
      supply = -1;
    }
    minerals += getMineralsPerSecond();
    vespene += getVespenePerSecond();
    timestamp += 1;
  }
}

// GET
float GameState::getMineralsPerSecond() const {
  if (mps == -1.0) {
    int nexi = 0;
    int active = 0;
    for (auto &u : freeUnits) {
      if (u.type == sc2::UNIT_TYPEID::PROTOSS_PROBE &&
          u.state == u.MINING_MINERALS) {
        active++;
      } else if (u.type == sc2::UNIT_TYPEID::PROTOSS_NEXUS &&
                 u.state != UnitInstance::BUILDING) {
        nexi++;
      }
    }
    for (auto &u : busyUnits) {
      if (u.type == sc2::UNIT_TYPEID::PROTOSS_NEXUS &&
          u.state != UnitInstance::BUILDING) {
        nexi++;
      }
    }

    int realactive = std::min(20 * nexi, active);
    // 115 frames : 0,9739130435
    mps = (0.896 * realactive);  // as measured for a RT average time of 125
                                 // frames 138 (3workers) : 0,8115942029
  }

  return mps;
}
float GameState::getVespenePerSecond() const {
  if (vps == -1.0) {
    vps = 0;
    for (auto &u : busyUnits) {
      if (u.type == sc2::UNIT_TYPEID::PROTOSS_PROBE &&
          u.state == u.MINING_VESPENE) {
        vps += 0.649;  // 138 (3workers) : 0,6492753623
      }
    }
  }
  return vps;
}
int GameState::getAvailableSupply() const {
  if (supply == -1) {
    auto &tech = TechTree::getTechTree();
    int sum = 0;

    for (auto &u : freeUnits) {
      sum += getFood(u, tech);
    }
    for (auto &u : attackUnits) {
      sum += getFood(u, tech);
    }
    for (auto &u : busyUnits) {
      sum += getFood(u, tech);
    }
    supply = sum;
  }

  return supply;
}
int GameState::getUsedSupply() const {
  auto &tech = TechTree::getTechTree();
  int sum = 0;
  for (auto &u : freeUnits) {
    auto f = getFood(u, tech);
    if (f < 0) {
      sum -= f;
    }
  }
  for (auto &u : attackUnits) {
    auto f = getFood(u, tech);
    if (f < 0) {
      sum -= f;
    }
  }
  for (auto &u : busyUnits) {
    auto f = getFood(u, tech);
    if (f < 0) {
      sum -= f;
    }
  }
  return sum;
}
int GameState::getMaxSupply() const {
  auto &tech = TechTree::getTechTree();
  int sum = 0;
  for (auto &u : freeUnits) {
    auto f = getFood(u, tech);
    if (f > 0) {
      sum += f;
    }
  }
  for (auto &u : busyUnits) {
    auto f = getFood(u, tech);
    if (f > 0) {
      sum += f;
    }
  }

  return sum;
}

// HAS
bool GameState::hasFreeUnit(UnitId unit) const {
  return std::any_of(freeUnits.begin(), freeUnits.end(), [unit](auto &u) {
    return (u.type == unit && u.state == u.FREE) ||
           (sc2util::IsWorkerType(unit) && u.state == u.MINING_MINERALS);
  });
}
bool GameState::hasFinishedUnit(UnitId unit) const {
  return std::any_of(busyUnits.begin(), busyUnits.end(),
                     [unit](auto &u) {
                       return (u.type == unit && u.state != u.BUILDING);
                     }) ||
         std::any_of(freeUnits.begin(), freeUnits.end(), [unit](auto &u) {
           return (u.type == unit && u.state != u.BUILDING);
         });
}


// ADD
void GameState::addUnit(UnitId unit) { addUnit(UnitInstance(unit)); }
void GameState::addUnit(const UnitInstance &unit) {
  if (unit.state == UnitInstance::MINING_MINERALS ||
      (unit.state == UnitInstance::FREE && sc2util::IsWorkerType(unit.type))) {
    freeUnits.push_back(unit);
    mps = -1.0f;
  } else if (unit.state == UnitInstance::FREE) {
    if (!sc2util::IsBuilding(unit.type)) {
      attackUnits.push_back(unit);
    } else {
      freeUnits.push_back(unit);
    }
  } else {
    busyUnits.push_back(unit);
    if (unit.state == UnitInstance::MINING_VESPENE) {
      vps = -1.0f;
    }
  }
  if (TechTree::getTechTree().getUnit(unit.type).food_provided != 0) {
    supply = -1;
  }
}


// Wait
bool GameState::waitForResources(int mins, int vesp,
                                 std::pair<int, int> *waited) {
  if (mins > minerals && getMineralsPerSecond() > 0) {
    int secs = (mins - minerals) / mps;
    if (((mins - minerals) / mps) - secs > 0) {
      secs += 1;
    }
    stepForward(secs);
    if (waited != nullptr) {
      waited->first = secs;
    }
    // std::cout << "Waited for minerals for " << secs << "s." << std::endl;
  } else if (mins > minerals) {
    return false;
  }
  if (vesp > vespene && getVespenePerSecond() > 0) {
    int secs = (vesp - vespene) / vps;
    if (((vesp - vespene) / vps) - secs > 0) {
      secs += 1;
    }
    stepForward(secs);
    if (waited != nullptr) {
      waited->second = secs;
    }
    // std::cout << "Waited for vespene for " << secs << "s." << std::endl;
  } else if (vesp > vespene) {
    return false;
  }
  return true;
}
bool GameState::waitforUnitCompletion(UnitId id) {
  auto it = std::find_if(busyUnits.begin(), busyUnits.end(), [id](auto &u) {
    return u.type == id && u.state == u.BUILDING;
  });
  if (it == busyUnits.end()) {
    return false;
  } else {
    // std::cout << "Waited for " <<
    // TechTree::getTechTree().getUnitById(id).name << " for " <<
    // it->time_to_free << "s." << std::endl;
    stepForward(it->time_to_free);
    return true;
  }
}
bool GameState::waitforUnitFree(UnitId id) {
  auto it = std::find_if(freeUnits.begin(), freeUnits.end(), [id](auto &u) {
    return u.type == id && u.state == u.FREE ||
           (sc2util::IsWorkerType(id) && u.state == u.MINING_MINERALS);
  });
  if (it != freeUnits.end()) {
    return true;
  }
  int index = 0;
  int best = -1;
  for (auto &u : busyUnits) {
    if (u.type == id && u.state != u.FREE && u.time_to_free != 0) {
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
    // TechTree::getTechTree().getUnitById(id).name << " to be free for " <<
    // units[best].time_to_free << "s." << std::endl;
    stepForward(busyUnits[best].time_to_free);
    return true;
  }
}
bool GameState::waitforAllUnitFree() {
  int index = 0;
  int best = -1;
  for (auto &u : busyUnits) {
    if (u.state != u.FREE && u.time_to_free != 0) {
      if (best == -1 || busyUnits[best].time_to_free < u.time_to_free) {
        best = index;
      }
    }
    index++;
  }
  if (best == -1) {
    return true;
  } else {
    // std::cout << "Waited for all to be free for " << units[best].time_to_free
    // << "s." << std::endl;
    stepForward(busyUnits[best].time_to_free);
    return true;
  }
}
bool GameState::waitforFreeSupply(int needed) {
  int cur = getAvailableSupply();
  if (cur >= needed) {
    return true;
  }
  auto it = std::find_if(busyUnits.begin(), busyUnits.end(), [](auto &u) {
    return u.type == sc2::UNIT_TYPEID::PROTOSS_PYLON ||
           u.type == sc2::UNIT_TYPEID::PROTOSS_NEXUS && u.state != u.FREE;
  });
  if (it == busyUnits.end()) {
    return false;
  }
  int index = 0;
  int best = -1;
  for (auto &u : busyUnits) {
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
    stepForward(busyUnits[best].time_to_free);
    return true;
  }
}

// Assign
bool GameState::assignProbe(UnitInstance::UnitState state) {
  int done = 0;
  for (auto it = freeUnits.begin(); it != freeUnits.end();) {
    bool erase = false;
    auto &u = *it;
    if (sc2util::IsWorkerType(u.type) &&
        (u.state == u.FREE || u.state == u.MINING_MINERALS)) {
      u.state = UnitInstance::MINING_VESPENE;
      vps = -1;
      mps = -1;
      busyUnits.push_back(u);
      erase = true;
      done++;
    }
    if (erase) {
      it = freeUnits.erase(it);
    } else {
      ++it;
    }
    if (done >= 3) break;
  }
  return done >= 3;
}
bool GameState::assignFreeUnit(UnitId type, UnitInstance::UnitState state,
                               int time) {
  for (auto it = freeUnits.begin(); it != freeUnits.end(); ++it) {
    auto &u = *it;
    if (type == u.type && u.state == u.FREE) {
      u.state = state;
      u.time_to_free = time;
      busyUnits.push_back(u);
      it = freeUnits.erase(it);
      return true;
    }
  }
  return false;
}

// Count
int GameState::countUnit(UnitId unit) const {
  return std::count_if(freeUnits.begin(), freeUnits.end(),
                       [unit](auto &u) { return u.type == unit; }) +
         std::count_if(busyUnits.begin(), busyUnits.end(),
                       [unit](auto &u) { return u.type == unit; }) +
         std::count_if(attackUnits.begin(), attackUnits.end(),
                       [unit](auto &u) { return u.type == unit; });
}
int GameState::countFreeUnit(UnitId unit) const {
  return std::count_if(freeUnits.begin(), freeUnits.end(), [unit](auto &u) {
    return u.type == unit && u.state == UnitInstance::FREE;
  });
}

int GameState::probesToSaturation() const {
  int nexi = countUnit(sc2::UNIT_TYPEID::PROTOSS_NEXUS);
  int ass = countUnit(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
  int prob = countUnit(sc2::UNIT_TYPEID::PROTOSS_PROBE);
  return (20 * nexi + 3 * ass) - prob;
}

// FIXME: to_string
namespace suboo {
std::string to_string(const UnitInstance::UnitState &state) {
  std::string status;
  switch (state) {
    case UnitInstance::BUILDING:
      status = "B";
      break;
    case UnitInstance::MINING_MINERALS:
      status = "M";
      break;
    case UnitInstance::MINING_VESPENE:
      status = "V";
      break;
    case UnitInstance::BUSY:
      status = "C";
      break;
    case UnitInstance::FREE:
    default:
      status = "F";
      break;
  }
  return status;
}
}

// print
void GameState::print(std::ostream &out) const {
  auto &tech = TechTree::getTechTree();
  std::unordered_map<int, std::unordered_map<UnitInstance::UnitState, int> >
      perUnitPerState;
  for (auto &u : freeUnits) {
    auto it = perUnitPerState.find(u.type);
    if (it == perUnitPerState.end()) {
      perUnitPerState[u.type] = {{u.state, 1}};
    } else {
      auto &perState = it->second;
      perState[u.state] += 1;
    }
  }
  for (auto &u : busyUnits) {
    auto it = perUnitPerState.find(u.type);
    if (it == perUnitPerState.end()) {
      perUnitPerState[u.type] = {{u.state, 1}};
    } else {
      auto &perState = it->second;
      perState[u.state] += 1;
    }
  }
  for (auto &u : attackUnits) {
    auto it = perUnitPerState.find(u.type);
    if (it == perUnitPerState.end()) {
      perUnitPerState[u.type] = {{u.state, 1}};
    } else {
      auto &perState = it->second;
      perState[u.state] += 1;
    }
  }
  for (auto it : perUnitPerState) {
    out << tech.getUnit(it.first).name << " ";
    for (auto jt : it.second) {
      out << jt.second << " x " << to_string(jt.first) << " ";
    }
    out << ",";
  }

  out << std::endl;
  out << "bank : minerals = " << minerals << "(" << getMineralsPerSecond() * 60
      << "/min)"
      << " vespene = " << vespene << "(" << getVespenePerSecond() * 60
      << "/min)"
      << " supply : " << getAvailableSupply() << ":" << getUsedSupply() << "/"
      << getMaxSupply() << std::endl;
}

