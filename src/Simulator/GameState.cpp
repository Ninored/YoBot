#include "Simulator/GameState.h"

namespace suboo {

GameState::GameState() {}

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
void step(int sec) {}

// GET
int GameState::getAvailabelSupply() const { return 1;}
int GameState::getMaxSupply() const { return 1;}
int GameState::getTimeStamp() const { return 1;}
float GameState::getMinerals() const { return 1;}
float GameState::getMps() const { return 1;}
float GameState::getVespene() const { return 1;}
float GameState::getVps() const { return 1;}
const std::list<UnitInstance*>& GameState::getFreeUnits() const {
  return freeUnits;
}
const std::list<UnitInstance*>& GameState::getBusyUnits() const {
  return busyUnits;
}

// Mutate unit
bool GameState::assignProbe(UnitInstance::UnitState state) { return true; }
bool GameState::assignFreeUnit(UnitId type, UnitInstance::UnitState state,
                               int time) {
  return true;
}

// Has
bool GameState::hasFreeUnit(UnitId unit) const { return true; }

// Wait function
bool GameState::waitforUnitFree(UnitId id) { return true; }
bool GameState::waitforFreeSupply(int nedded) { return true; }
bool GameState::waitforAllUnitFree() { return true; }

// Count
int GameState::countUnit(UnitId id) const { return 1; }
int GameState::countFreeUnit(UnitId id) const { return 1; }

// Tools
int GameState::probesToStauration() const { return 1; }

std::ostream& operator<<(std::ostream& os, const GameState& state) {
  return os;
}

}  // namespace suboo
