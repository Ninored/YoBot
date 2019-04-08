#include "Simulator/GameState.h"
#include <algorithm>
#include "Simulator/UnitTypes.h"

namespace suboo {

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
  for (auto u : freeUnits) {
    if (u.type == sc2::UNIT_TYPEID::PROTOSS_PROBE &&
        u.state == u.MINING_VESPENE)
      vps += 0.649f;
  }
}
void GameState::calculateSupply() {
  supply = 0;
  auto& tech = TechTree::getTechTree();

  for (auto& u : freeUnits) supply += tech.getUnit(u.type).food_provided;
  for (auto& u : busyUnits) supply += tech.getUnit(u.type).food_provided;
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
      it->time_to_free--;
      if (it->time_to_free <= 0) {
        setFree(it);
      } else {
        ++it;
      }
    }

    minerals += getMps();
    vespenes += getVps();
    timestamp += 1;
  }
}

// GET
int GameState::getAvailabelSupply() const { return supply; }
int GameState::getMaxSupply() const { return supply; }
int GameState::getTimeStamp() const { return timestamp; }
float& GameState::getMinerals() { return minerals; }
float GameState::getMps() const { return mps; }
float& GameState::getVespene() { return vespenes; }
float GameState::getVps() const { return vps; }
const std::vector<UnitInstance>& GameState::getFreeUnits() const {
  return freeUnits;
}
const std::vector<UnitInstance>& GameState::getBusyUnits() const {
  return busyUnits;
}

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
bool GameState::assignProbe(UnitInstance::UnitState state) {
  calculateMps();
  calculateVps();
  return true;
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

// Wait function
bool GameState::waitforUnitCompletion(UnitId id) {
  auto it = std::find_if(busyUnits.begin(), busyUnits.end(), [id](auto& u) {
    return u.type == id && u.state == u.BUILDING;
  });
  if (it == busyUnits.end()) return false;


  std::cout << "[waitforUnitCompletio] wait for" << (int)id << "): " << it->time_to_free << std::endl;
  step(it->time_to_free);
  return true;
}
bool GameState::waitforUnitFree(UnitId id) {
  /* Search builder */
  auto it = std::min_element(std::begin(busyUnits), std::end(busyUnits),
                        [id](const UnitInstance& a, const UnitInstance& b) {
                          return a.time_to_free < b.time_to_free;
                        });

  if (it == busyUnits.end()) return false;

  std::cout << "[waitforUnitFree] wait for (" << (int)id << "): " << it->time_to_free << std::endl;
  step(it->time_to_free);

  return true;
}
bool GameState::waitforFreeSupply(int nedded) { return true; }
bool GameState::waitforAllUnitFree() {
  int time_to_free = 0;
  for (auto u : busyUnits) 
    if (time_to_free < u.time_to_free )
      time_to_free = u.time_to_free;

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
  return freeUnits.size() + busyUnits.size() + attackUnits.size();
}
int GameState::countFreeUnit(UnitId id) const { return freeUnits.size(); }

// Tools
int GameState::probesToStauration() const { return 1; }

std::ostream& operator<<(std::ostream& os, const GameState& state) {
  os << "[GS:" << state.timestamp << "]" << std::endl
     << "\t(min: " << state.minerals << ", vesp: " << state.vespenes
     << ", mps:" << state.mps << ", vps:" << state.vps << ")" << std::endl;

  os << "\t[freeUnits]: (";
  for (auto u : state.freeUnits) {
    os << u.type << ", ";
  }
  os << " )"<< std::endl;

  os << "\t[busyUnits]: (";
  for (auto u : state.busyUnits) {
    os << u.type << ", ";
  }
  os << " )"<< std::endl;
  return os;
}

}  // namespace suboo
