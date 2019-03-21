#include <iostream>
#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"
#include "sc2utils/sc2_manage_process.h"
#include "sc2utils/sc2_arg_parser.h"
#include "BuildOrder/BuildOrder.h"
#include "BuildOrder/BOBuilder.h"
#include "TechTree/TechTree.h"

using namespace sc2;
using namespace suboo;

int main(int argc, char* argv[])
{
  // Loading techtree
  TechTree::getTechTree().deserialize("techtree_.json");


	BOBuilder builder;

	BuildGoal goal(0); // ASAP
	goal.addUnit(sc2::UNIT_TYPEID::PROTOSS_IMMORTAL, 1);
	//goal.addUnit(UnitId::PROTOSS_DISRUPTOR, 1);
  //goal.addUnit(sc2::UNIT_TYPEID::PROTOSS_CARRIER, 0);
	//goal.addUnit(UnitId::PROTOSS_GATEWAY, 3);
	//goal.addUnit(UnitId::PROTOSS_STARGATE, 3);
	//goal.addUnit(sc2::UNIT_TYPEID::PROTOSS_OBSERVER, 0);
	//goal.addUnit(sc2::UNIT_TYPEID::PROTOSS_PHOENIX, 0);
	//goal.addUnit(sc2::UNIT_TYPEID::PROTOSS_ZEALOT, 00); //goal.addUnit(UnitId::PROTOSS_NEXUS, 1);

	builder.addGoal(goal);
	goal.print(std::cout);

	BuildOrder bo = builder.computeBO();
	std::cout << "Initial realizable :" << std::endl;
	bo.print(std::cout);
	auto boopt = builder.improveBO(bo, 2);

	std::cout << "Final realizable :" << std::endl;
	boopt.print(std::cout);

	auto boopt2 = builder.improveBO(boopt, 5);
	std::cout << "Final realizable :" << std::endl;
	boopt2.print(std::cout);


	if (true) {
		BuildOrder bo;
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PYLON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_GATEWAY);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_CYBERNETICSCORE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
		bo.addItem(BuildItem::TRANSFER_VESPENE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PYLON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PYLON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PYLON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PROBE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_STARGATE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PYLON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PHOENIX);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ASSIMILATOR);
		bo.addItem(BuildItem::TRANSFER_VESPENE);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PYLON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PHOENIX);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PHOENIX);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_ZEALOT);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_FLEETBEACON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_PYLON);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_OBSERVER);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_IMMORTAL);
		bo.addItem(sc2::UNIT_TYPEID::PROTOSS_CARRIER);
		if (!timeBO(bo)) {
			auto boe = BOBuilder::enforcePrereq(bo);
			timeBO(boe);
			std::cout << "Patch was necessary :" << std::endl;
			bo = boe;
		}
		std::cout << "build by voxel :" << std::endl;
		bo.print(std::cout);

		auto boopt2 = builder.improveBO(bo, 3);

		std::cout << "Final realizable :" << std::endl;
		boopt2.print(std::cout);
	}

	std::string s;
	std::cin >> s;

	return 0;
}