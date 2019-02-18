#include "TechTree/TechTreeBot.h"
#include <algorithm>
#include <iostream>
#include <vector>
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
    state.emplace_back(UnitInstance(u->unit_type));
  }

  Debug()->DebugGiveAllResources();
  Debug()->DebugGiveAllTech();
  Debug()->DebugGiveAllUpgrades();

  auto point = info.start_locations[0];
  for (const sc2::UnitTypeData &unitdesc : types) {
    point = point + sc2::Point3D(3, 0, 0);
    while (!Query()->Placement(sc2::ABILITY_ID::BUILD_GATEWAY, point)) {
      point = point + sc2::Point3D(3, 0, 0);
      if (point.x >= info.width) point = sc2::Point3D(0, point.y + 3, 0);

      if (point.y >= info.height) return;
      Debug()->DebugCreateUnit(unitdesc.unit_type_id, point,
                               info.player_info[0].player_id);
      Debug()->DebugCreateUnit(sc2::UNIT_TYPEID::PROTOSS_PYLON,
                               point + sc2::Point3D(0, -3, 0),
                               info.player_info[0].player_id);
    }
  }
  Debug()->SendDebug();
  std::cout << "OnGameStart finished..." << std::endl;
}

void TechTreeBot::OnStep() {
  if (Observation()->GetGameLoop() != 5) return;

  auto &types = Observation()->GetUnitTypeData();
  auto &abilities = Observation()->GetAbilityData();

  std::unordered_map<int, std::vector<AbilityId>> unitToAbilities;

  for (auto u : Observation()->GetUnits(sc2::Unit::Alliance::Self)) {
    auto &abilities = Query()->GetAbilitiesForUnit(u, true);
    unitToAbilities.insert({(int)u->unit_type, std::vector<AbilityId>()});
    for (auto ab : abilities.abilities) {
      unitToAbilities[(int)u->unit_type].push_back((int)ab.ability_id);
    }
  }

  for (const sc2::UnitTypeData &unitdesc : types) {
    TechTree::getTechTree().addUnit({
        unitdesc.unit_type_id,
        unitdesc.name,
        unitdesc.mineral_cost,
        unitdesc.vespene_cost,
        (int)unitdesc.food_required + (int)unitdesc.food_provided,
        unitToAbilities[unitdesc.unit_type_id],
        unitdesc.tech_requirement,
    });
  }

  std::cout << TechTree::getTechTree() << std::endl;
}

}  // namespace suboo