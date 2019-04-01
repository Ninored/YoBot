#include "Simulator/GameState.h"

namespace suboo {

GameState::GameState(const std::vector<UnitInstance>& units, int m, int v)
    : minerals(m), vespenes(v), mps(0), vps(0), timestamp(0) {
  for (auto u : units) {
    busyUnits.push_back(&allUnits.emplace_back(UnitInstance(u)));
  }
  calculateMps();
  calculateVps();
  calculateSupply();
}

// Calculate
void GameState::calculateMps() {
  for (auto u : busyUnits) {
    if (u->type == sc2::UNIT_TYPEID::PROTOSS_PROBE &&
        u->state == u->MINING_MINERALS)
      mps += 0.896f;
  }
}
void GameState::calculateVps() {
  for (auto u : busyUnits) {
    if (u->type == sc2::UNIT_TYPEID::PROTOSS_PROBE &&
        u->state == u->MINING_VESPENE)
      vps += 0.649f;
  }
}
void GameState::calculateSupply() {
  auto& tech = TechTree::getTechTree();
  for (auto& u : allUnits) {
    supply += tech.getUnit(u.type).food_provided;
  }
}

// Mutate State
bool GameState::addUnit(UnitId unit) { return addUnit(UnitInstance(unit)); }
bool GameState::addUnit(const UnitInstance& unit) {
  UnitInstance& u = allUnits.emplace_back(unit.type);

  switch (u.state) {
    case UnitInstance::MINING_MINERALS:
      busyUnits.push_back(&u);
      mps = -1.0f;
      break;

    case UnitInstance::MINING_VESPENE:
      busyUnits.push_back(&u);
      vps = -1.0f;
      break;

    case UnitInstance::BUILDING:
      busyUnits.push_back(&u);
      break;

    case UnitInstance::FREE:
      freeUnits.push_back(&u);
      break;

    default:
      std::cerr << "Unimplemented unit status !" << std::endl;
      return false;
  }

  if (TechTree::getTechTree().getUnit(u.type).food_provided != 0) supply = -1;

  return true;
}

// Step function
void GameState::step(int sec) {
  for (int i = 0; i < sec; ++i) {
    /*
    for (auto it = busyUnits.begin(); it != busyUnits.end(); ++it) {
      (*it)->time_to_free--;
      if ((*it)->time_to_free < 0) {
        setFree(*it);
        busyUnits.erase(it);
      }
    }*/
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
const std::list<UnitInstance*>& GameState::getFreeUnits() const {
  return freeUnits;
}
const std::list<UnitInstance*>& GameState::getBusyUnits() const {
  return busyUnits;
}

// SET
void GameState::setFree(UnitInstance* inst) {
  freeUnits.push_front(inst);
  calculateMps();
  calculateVps();
}

// Mutate unit
bool GameState::assignProbe(UnitInstance::UnitState state) {
  calculateMps();
  calculateVps();
  return true;
}
bool GameState::assignFreeUnit(UnitId type, UnitInstance::UnitState state,
                               int time) {
  calculateMps();
  calculateVps();
  return true;
}

// Has
bool GameState::hasFreeUnit(UnitId unit) const {
  for (auto u : freeUnits)
    if (u->state == u->FREE)
      return true;
  return false;
}

// Wait function
bool GameState::waitforUnitFree(UnitId id) { return true; }
bool GameState::waitforFreeSupply(int nedded) { return true; }
bool GameState::waitforAllUnitFree() { return true; }
std::pair<int, int> GameState::waitForRessources(int mineral, int vespene) {
  std::pair<int, int> res{0, 0};

  // wait minerals
  if (mineral > minerals) {
    int msecs = (-mineral - minerals) / mps;
    std::cout << "Wait minerals for: " << msecs << std::endl;
    step(msecs);
    res.first = msecs;
  }

  // wait vespene
  if (vespene > vespenes) {
    int vsecs = (-vespene - vespenes) / vps;
    std::cout << "Wait vespene for: " << vsecs << std::endl;
    step(vsecs);
    res.second = vsecs;
  }

  return res;
}

// Count
int GameState::countUnit(UnitId id) const { return 1; }
int GameState::countFreeUnit(UnitId id) const { return 1; }

// Tools
int GameState::probesToStauration() const { return 1; }

std::ostream& operator<<(std::ostream& os, const GameState& state) {
  return os;
}

}  // namespace suboo
