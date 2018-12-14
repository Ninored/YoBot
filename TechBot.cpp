#include "TechBot.h"
#include "sc2api/sc2_action.h"
#include "sc2api/sc2_interfaces.h"
#include "UnitTypes.h"
#include "MapTopology.h"
#include <iostream>
#include <fstream>


bool isRelevant(const sc2::UnitTypeData & unitdesc) {
	return unitdesc.available && unitdesc.race == sc2::Race::Protoss
		&& !(unitdesc.name.find("Weapon") != std::string::npos
			|| unitdesc.name.find("MP") != std::string::npos
			|| unitdesc.name.find("SkinPreview") != std::string::npos
			|| unitdesc.name.find("Interceptors") != std::string::npos);
}

void suboo::TechBot::OnGameStart()
{
	
	auto & types = Observation()->GetUnitTypeData();
	auto & abilities = Observation()->GetAbilityData();

	MapTopology map;
	map.init(Observation(), Query(), Debug());

	auto & info = Observation()->GetGameInfo();

	// inspect and create an initial state
	std::vector<UnitInstance> state;
	for (auto u : Observation()->GetUnits(sc2::Unit::Alliance::Self)) {
		state.emplace_back(UnitInstance(u->unit_type));
	}
	initial = GameState(state,Observation()->GetMinerals(), Observation()->GetVespene());

	// time to create some units
	Debug()->DebugGiveAllResources();
	Debug()->DebugGiveAllTech();
	Debug()->DebugGiveAllUpgrades();
	auto point = map.getPosition(map.ally,map.main);
	for (const sc2::UnitTypeData & unitdesc : types) {
		if (isRelevant(unitdesc)) {
			point = point + sc2::Point3D(3, 0, 0);
			while (! map.PlacementB(info, point,3)) {
				point = point + sc2::Point3D(3, 0, 0);
				if (point.x >= info.width) {
					point = sc2::Point3D(0, point.y + 3, 0);
				}
				if (point.y >= info.height) {
					return;
				}
			}
		
			Debug()->DebugCreateUnit(unitdesc.unit_type_id, point,info.player_info[0].player_id);
			// for power, or we don't get build/tech abilities
			Debug()->DebugCreateUnit(sc2::UNIT_TYPEID::PROTOSS_PYLON, point + sc2::Point3D(0,-3,0), info.player_info[0].player_id);
		}
	}
	Debug()->SendDebug();


}

void suboo::TechBot::OnStep()
{
	if (Observation()->GetGameLoop() == 5) {

		auto & types = Observation()->GetUnitTypeData();
		auto & abilities = Observation()->GetAbilityData();

		std::unordered_map<int, int> abilityToUnit;
		for (auto u : Observation()->GetUnits(sc2::Unit::Alliance::Self)) {
			auto & abilities = Query()->GetAbilitiesForUnit(u,true);
			for (auto & ab : abilities.abilities) {
				abilityToUnit[(int)ab.ability_id] = (int) u->unit_type;
			}
		}

		std::ofstream out("TechTree" + version + ".cpp");

		out << "#include \"BuildOrder.h\"\n";
		out << "namespace suboo {\n";

		out << "TechTree::TechTree() :\n";
		out << "	initial({";
		for (auto & u : initial.getUnits()) {
			out << " UnitInstance( (UnitId)" << (int) u.type << "),\n";
		}
		out << "	}) {\n";
		out << "  units = {\n";


		for (const sc2::UnitTypeData & unitdesc : types) {

			if (isRelevant(unitdesc)) {
				if (sc2util::IsBuilding(unitdesc.unit_type_id))
				{	// protoss buildings are all produced by a probe walking there 
					int traveltime = 4; // default estimate is 4 seconds to reach build site
					if (sc2util::IsCommandStructure(unitdesc.unit_type_id)) {
						traveltime = 10; // CC require to move to an expansion, that takes about 10 secs
					}
					auto builderid = abilityToUnit[(int)unitdesc.ability_id];
					if (builderid != 0) {
						out << "{"
							"	(UnitId)" << unitdesc.unit_type_id << ",  // ID" << std::endl <<
							"	\"" << unitdesc.name << "\", // name" << std::endl <<
							"	" << unitdesc.mineral_cost << ", // gold" << std::endl <<
							"	" << unitdesc.vespene_cost << ", // gas" << std::endl <<
							"	" << unitdesc.food_provided - unitdesc.food_required << ",  // food" << std::endl <<
							"	(UnitId)" << (int)builderid << ",  // builder unit  " << std::endl <<
							"	(UnitId)" << (int)unitdesc.tech_requirement << ",  // tech requirement  " << std::endl <<
							"	" << unitdesc.build_time << ", // build time" << std::endl <<
							"	" << traveltime << ",   // travel time" << std::endl <<
							"	Unit::TRAVEL  // probe behavior" << std::endl <<
							"}," << std::endl;
					}
				}
				else
				{
					// it's a moving unit, produced from a production building 
					auto builderid = abilityToUnit[(int)unitdesc.ability_id];
					// no mothership (only one at atime on map = no ability avail on nexus),
					if (unitdesc.name == "Mothership") {
						builderid = (int) sc2::UNIT_TYPEID::PROTOSS_NEXUS;
					}
					// TODO no archons : need two templar to have the ability
					if (builderid != 0) {
						out << "{"
							"	(UnitId)" << unitdesc.unit_type_id << ",  // ID" << std::endl <<
							"	\"" << unitdesc.name << "\", // name" << std::endl <<
							"	" << unitdesc.mineral_cost << ", // gold" << std::endl <<
							"	" << unitdesc.vespene_cost << ", // gas" << std::endl <<
							"	" << unitdesc.food_provided - unitdesc.food_required << ",  // food" << std::endl <<
							"	(UnitId)" << (int)builderid << ",  // builder unit  " << std::endl <<
							"	(UnitId)" << (int)unitdesc.tech_requirement << ", // tech requirement " << std::endl <<
							"	" << unitdesc.build_time << ", // build time" << std::endl <<
							"	" << 0 << ",   // travel time" << std::endl <<
							"	Unit::BUSY  // producer behavior" << std::endl <<
							"}," << std::endl;
					}
				}
			}
		};


		out << "  }; // end units\n";
		out << "} //end ctor \n";
		out << "} //end ns \n";
		out.close();
	}
}
