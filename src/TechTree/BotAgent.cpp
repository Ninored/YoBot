#include "BotAgent.hpp"
#include <vector>
#include "TechTree/TechTree.hpp"
#include "sc2api/sc2_action.h"
#include "sc2api/sc2_interfaces.h"

BotAgent::BotAgent() {}

BotAgent::~BotAgent() {}

void BotAgent::OnGameStart() {
  auto & types = Observation()->GetUnitTypeData();    // Type Observator 
  auto & abilities = Observation()->GetAbilityData(); // Ability Observator
  auto & info = Observation()->GetGameInfo();         // GameInfo Observator

  // Get All available unit types and add them to our instances variable
  std::vector<suboo::UnitInstance> instances;
  for(auto u : Observation()->GetUnits(sc2::Unit::Alliance::Self)) {
    instances.emplace_back(suboo::UnitInstance{{u->unit_type}});
  }

  Debug()->DebugGiveAllResources();
	Debug()->DebugGiveAllTech();
	Debug()->DebugGiveAllUpgrades();

  Debug()->SendDebug();


}

void BotAgent::OnStep() {
  auto &types = Observation()->GetUnitTypeData();    // Type Observator
  auto &abilities = Observation()->GetAbilityData(); // Ability Observator
  auto &info = Observation()->GetGameInfo();         // GameInfo Observator

  
}