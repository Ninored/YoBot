#include "Simulator/UnitTypes.h"

using namespace sc2;

namespace sc2util {

bool IsMineral(sc2::UNIT_TYPEID type) {
  switch (type) {
    case UNIT_TYPEID::NEUTRAL_MINERALFIELD:
    case UNIT_TYPEID::NEUTRAL_MINERALFIELD750:
    case UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD:
    case UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD750:
    case UNIT_TYPEID::NEUTRAL_PURIFIERMINERALFIELD:
    case UNIT_TYPEID::NEUTRAL_PURIFIERMINERALFIELD750:
    case UNIT_TYPEID::NEUTRAL_PURIFIERRICHMINERALFIELD:
    case UNIT_TYPEID::NEUTRAL_PURIFIERRICHMINERALFIELD750:
    case UNIT_TYPEID::NEUTRAL_LABMINERALFIELD:
    case UNIT_TYPEID::NEUTRAL_LABMINERALFIELD750:
    case UNIT_TYPEID::NEUTRAL_BATTLESTATIONMINERALFIELD:
    case UNIT_TYPEID::NEUTRAL_BATTLESTATIONMINERALFIELD750:
      return true;
  }
  return false;
}

bool IsVespene(sc2::UNIT_TYPEID type) {
  switch (type) {
    case UNIT_TYPEID::NEUTRAL_VESPENEGEYSER:
    case UNIT_TYPEID::NEUTRAL_PROTOSSVESPENEGEYSER:
    case UNIT_TYPEID::NEUTRAL_SHAKURASVESPENEGEYSER:
    case UNIT_TYPEID::NEUTRAL_RICHVESPENEGEYSER:
    case UNIT_TYPEID::NEUTRAL_PURIFIERVESPENEGEYSER:
    case UNIT_TYPEID::NEUTRAL_SPACEPLATFORMGEYSER:
      return true;
  }
  return false;
}

bool IsWorkerType(sc2::UNIT_TYPEID type) {
  switch (type) {
    case sc2::UNIT_TYPEID::TERRAN_SCV:
    case sc2::UNIT_TYPEID::TERRAN_MULE:
    case sc2::UNIT_TYPEID::PROTOSS_PROBE:
    case sc2::UNIT_TYPEID::ZERG_DRONE:
    case sc2::UNIT_TYPEID::ZERG_DRONEBURROWED:
      return true;
    default:
      return false;
  }
}

bool IsCommandStructure(sc2::UNIT_TYPEID type) {
  switch (type) {
    case UNIT_TYPEID::TERRAN_COMMANDCENTER:
    case UNIT_TYPEID::TERRAN_COMMANDCENTERFLYING:
    case UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
    case UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
    case UNIT_TYPEID::TERRAN_ORBITALCOMMANDFLYING:
    case UNIT_TYPEID::PROTOSS_NEXUS:
    case UNIT_TYPEID::ZERG_HATCHERY:
    case UNIT_TYPEID::ZERG_LAIR:
    case UNIT_TYPEID::ZERG_HIVE:
      return true;
    default:
      return false;
  }
}

bool isStaticDefense(const sc2::UNIT_TYPEID type) {
  switch (type) {
    case sc2::UNIT_TYPEID::ZERG_SPINECRAWLER:
    case sc2::UNIT_TYPEID::ZERG_SPINECRAWLERUPROOTED:
    case sc2::UNIT_TYPEID::PROTOSS_PHOTONCANNON:
    case sc2::UNIT_TYPEID::PROTOSS_SHIELDBATTERY:
    case sc2::UNIT_TYPEID::TERRAN_BUNKER:
    case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
      return true;
  }
  return false;
}

bool IsRangedUnit(const sc2::UNIT_TYPEID type) {
  switch (type) {
    case UNIT_TYPEID::TERRAN_MARINE:
    case UNIT_TYPEID::TERRAN_MARAUDER:
    case UNIT_TYPEID::PROTOSS_ADEPT:
    case UNIT_TYPEID::PROTOSS_STALKER:
    case UNIT_TYPEID::ZERG_ROACH:
      return true;
  }
  return false;
}

bool IsBuilding(const sc2::UNIT_TYPEID type) {
  if (IsCommandStructure(type)) {
    return true;
  }
  switch (type) {
      // Terran
    case sc2::UNIT_TYPEID::TERRAN_ARMORY:
    case sc2::UNIT_TYPEID::TERRAN_BARRACKS:
    case sc2::UNIT_TYPEID::TERRAN_BARRACKSFLYING:
    case sc2::UNIT_TYPEID::TERRAN_BARRACKSREACTOR:
    case sc2::UNIT_TYPEID::TERRAN_BUNKER:
    case sc2::UNIT_TYPEID::TERRAN_BARRACKSTECHLAB:
    case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
    case sc2::UNIT_TYPEID::TERRAN_ENGINEERINGBAY:
    case sc2::UNIT_TYPEID::TERRAN_FACTORY:
    case sc2::UNIT_TYPEID::TERRAN_FACTORYFLYING:
    case sc2::UNIT_TYPEID::TERRAN_FACTORYREACTOR:
    case sc2::UNIT_TYPEID::TERRAN_FACTORYTECHLAB:
    case sc2::UNIT_TYPEID::TERRAN_FUSIONCORE:
    case sc2::UNIT_TYPEID::TERRAN_GHOSTACADEMY:
    case sc2::UNIT_TYPEID::TERRAN_MISSILETURRET:
    case sc2::UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
    case sc2::UNIT_TYPEID::TERRAN_REFINERY:
    case sc2::UNIT_TYPEID::TERRAN_SENSORTOWER:
    case sc2::UNIT_TYPEID::TERRAN_STARPORT:
    case sc2::UNIT_TYPEID::TERRAN_STARPORTFLYING:
    case sc2::UNIT_TYPEID::TERRAN_STARPORTREACTOR:
    case sc2::UNIT_TYPEID::TERRAN_STARPORTTECHLAB:
    case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOT:
    case sc2::UNIT_TYPEID::TERRAN_SUPPLYDEPOTLOWERED:
    case sc2::UNIT_TYPEID::TERRAN_REACTOR:
    case sc2::UNIT_TYPEID::TERRAN_TECHLAB:

      // Zerg
    case sc2::UNIT_TYPEID::ZERG_BANELINGNEST:
    case sc2::UNIT_TYPEID::ZERG_CREEPTUMOR:
    case sc2::UNIT_TYPEID::ZERG_CREEPTUMORBURROWED:
    case sc2::UNIT_TYPEID::ZERG_CREEPTUMORQUEEN:
    case sc2::UNIT_TYPEID::ZERG_EVOLUTIONCHAMBER:
    case sc2::UNIT_TYPEID::ZERG_EXTRACTOR:
    case sc2::UNIT_TYPEID::ZERG_GREATERSPIRE:
    case sc2::UNIT_TYPEID::ZERG_HYDRALISKDEN:
    case sc2::UNIT_TYPEID::ZERG_INFESTATIONPIT:
    case sc2::UNIT_TYPEID::ZERG_LURKERDENMP:
    case sc2::UNIT_TYPEID::ZERG_NYDUSCANAL:
    case sc2::UNIT_TYPEID::ZERG_NYDUSNETWORK:
    case sc2::UNIT_TYPEID::ZERG_SPINECRAWLER:
    case sc2::UNIT_TYPEID::ZERG_SPINECRAWLERUPROOTED:
    case sc2::UNIT_TYPEID::ZERG_SPAWNINGPOOL:
    case sc2::UNIT_TYPEID::ZERG_SPIRE:
    case sc2::UNIT_TYPEID::ZERG_SPORECRAWLER:
    case sc2::UNIT_TYPEID::ZERG_SPORECRAWLERUPROOTED:
    case sc2::UNIT_TYPEID::ZERG_ULTRALISKCAVERN:

      // Protoss
    case sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR:
    case sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE:
    case sc2::UNIT_TYPEID::PROTOSS_DARKSHRINE:
    case sc2::UNIT_TYPEID::PROTOSS_FLEETBEACON:
    case sc2::UNIT_TYPEID::PROTOSS_FORGE:
    case sc2::UNIT_TYPEID::PROTOSS_PHOTONCANNON:
    case sc2::UNIT_TYPEID::PROTOSS_SHIELDBATTERY:
    case sc2::UNIT_TYPEID::PROTOSS_GATEWAY:
    case sc2::UNIT_TYPEID::PROTOSS_PYLON:
    case sc2::UNIT_TYPEID::PROTOSS_PYLONOVERCHARGED:
    case sc2::UNIT_TYPEID::PROTOSS_ROBOTICSBAY:
    case sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY:
    case sc2::UNIT_TYPEID::PROTOSS_STARGATE:
    case sc2::UNIT_TYPEID::PROTOSS_TEMPLARARCHIVE:
    case sc2::UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL:
    case sc2::UNIT_TYPEID::PROTOSS_WARPGATE:
      return true;
  }
  return false;
}

bool IsArmyUnitType(const sc2::UNIT_TYPEID type) {
  if (IsWorkerType(type)) {
    return false;
  }
  if (type == sc2::UNIT_TYPEID::ZERG_OVERLORD) {
    return false;
  }  // Excluded here the overlord transport etc to count them as army unit
  if (IsBuilding(type)) {
    return false;
  }
  if (type == sc2::UNIT_TYPEID::ZERG_EGG) {
    return false;
  }
  if (type == sc2::UNIT_TYPEID::ZERG_LARVA) {
    return false;
  }
  if (type == sc2::UNIT_TYPEID::PROTOSS_ADEPTPHASESHIFT) {
    return false;
  }
  return true;
}

bool isChronoed(const sc2::Unit *gw) {
  for (auto &buff : gw->buffs) {
    if (buff == sc2::BUFF_ID::TIMEWARPPRODUCTION) {
      return true;
    }
  }
  return false;
}

float getRange(const Unit *z, const sc2::UnitTypes &types) {
  auto &arms = types.at(static_cast<uint32_t>(z->unit_type)).weapons;
  if (arms.empty()) {
    return z->radius;
  }
  float attRange = arms.front().range + z->radius;

  return attRange;
}

}  // namespace sc2util