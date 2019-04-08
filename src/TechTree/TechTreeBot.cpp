#include "TechTree/TechTreeBot.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include "TechTree/TechTree.h"
#include "sc2api/sc2_action.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_map_info.h"

namespace suboo {

TechTreeBot::TechTreeBot() {}

TechTreeBot::~TechTreeBot() {}

void TechTreeBot::OnGameStart() {
  std::cout << "TechTree Agent Started ..." << std::endl;

  auto &types = Observation()->GetUnitTypeData();
  auto &abilities = Observation()->GetAbilityData();
  auto &info = Observation()->GetGameInfo();

  // inspect and create an initial state
  std::vector<UnitInstance> state;
  for (auto u : Observation()->GetUnits(sc2::Unit::Alliance::Self)) {
    /* FIXME: Specificity */
    if (u->unit_type == sc2::UNIT_TYPEID::PROTOSS_PROBE)
      state.emplace_back(UnitInstance(u->unit_type, UnitInstance::MINING_MINERALS, 0));
    else
      state.emplace_back(UnitInstance(u->unit_type));
  
  }

  TechTree::getTechTree().setInitialState(state, Observation()->GetMinerals(),
                                          Observation()->GetVespene());


  Debug()->DebugGiveAllResources();
  Debug()->DebugGiveAllTech();
  Debug()->DebugGiveAllUpgrades();

  auto point = info.start_locations[0];
  for (const sc2::UnitTypeData &unitdesc : types) {
    if (std::string(sc2::UnitTypeToName(unitdesc.unit_type_id))
            .compare(0, 6, "PROTOS") != 0)
      continue;

    while (!Query()->Placement(sc2::ABILITY_ID::BUILD_PYLON, point)) {
      point = point + sc2::Point2D(3, 0);
      if (point.x >= info.width) point = sc2::Point2D(0, point.y + 3);
      if (point.y >= info.height) return;
    }

    Debug()->DebugCreateUnit(unitdesc.unit_type_id, point,
                             info.player_info[0].player_id);
    Debug()->DebugCreateUnit(sc2::UNIT_TYPEID::PROTOSS_PYLON,
                             point + sc2::Point2D(0, -3),
                             info.player_info[0].player_id);
  }
  Debug()->SendDebug();
  std::cout << "OnGameStart finished..." << std::endl;
}

void TechTreeBot::OnStep() {
  if (Observation()->GetGameLoop() != 5) return;

  auto &types = Observation()->GetUnitTypeData();
  auto &abilities = Observation()->GetAbilityData();

  std::unordered_map<int, UnitId> abilityToUnits;
  std::unordered_map<int, std::set<AbilityId>> unitToAbilities;
  for (auto u : Observation()->GetUnits(sc2::Unit::Alliance::Self)) {
    auto &abilities = Query()->GetAbilitiesForUnit(u, true);
    for (auto ab : abilities.abilities) {
      abilityToUnits[(int)ab.ability_id] = u->unit_type;
    }
  }

  for (const sc2::UnitTypeData &unitdesc : types) {
    // Filter unwanted units, we are focused on protos for the moment
    if (std::string(sc2::UnitTypeToName(unitdesc.unit_type_id))
            .compare(0, 6, "PROTOS") != 0)
      continue;

    UnitId tech_req = unitdesc.tech_requirement;
    if (tech_req == sc2::UNIT_TYPEID::PROTOSS_NEXUS)
      tech_req = sc2::UNIT_TYPEID::PROTOSS_PYLON;

    TechTree::getTechTree().addUnit({
      unitdesc.unit_type_id,
      unitdesc.name,
      unitdesc.mineral_cost,
      unitdesc.vespene_cost,
      (int)unitdesc.food_provided - (int)unitdesc.food_required,
      unitToAbilities[(int)unitdesc.unit_type_id],
      tech_req,
      abilityToUnits[(int)unitdesc.ability_id],
      (int)((float)unitdesc.build_time / 22.4),
      Unit::Status::TRAVELLING,
    });
  }
  // std::cout << TechTree::getTechTree() << std::endl;
  //std::cout << TechTree::getTechTree().serialize() << std::endl;
}

}  // namespace suboo