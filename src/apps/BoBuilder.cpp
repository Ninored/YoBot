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
	BOBuilder builder;

	BuildGoal goal(0); // ASAP
	goal.addUnit(UnitId::PROTOSS_IMMORTAL, 1);
	//goal.addUnit(UnitId::PROTOSS_DISRUPTOR, 1);
	goal.addUnit(UnitId::PROTOSS_CARRIER, 1);
	//goal.addUnit(UnitId::PROTOSS_GATEWAY, 3);
	//goal.addUnit(UnitId::PROTOSS_STARGATE, 3);
	goal.addUnit(UnitId::PROTOSS_OBSERVER, 1);
	goal.addUnit(UnitId::PROTOSS_PHOENIX, 3);
	goal.addUnit(UnitId::PROTOSS_ZEALOT, 10); //goal.addUnit(UnitId::PROTOSS_NEXUS, 1);

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
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_PYLON);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_GATEWAY);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_GATEWAY);
		bo.addItem(UnitId::PROTOSS_CYBERNETICSCORE);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_ASSIMILATOR);
		bo.addItem(TRANSFER_VESPENE);
		bo.addItem(UnitId::PROTOSS_PYLON);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PYLON);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PYLON);
		bo.addItem(UnitId::PROTOSS_PROBE);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_STARGATE);
		bo.addItem(UnitId::PROTOSS_PYLON);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PHOENIX);
		bo.addItem(UnitId::PROTOSS_ASSIMILATOR);
		bo.addItem(TRANSFER_VESPENE);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PYLON);
		bo.addItem(UnitId::PROTOSS_PHOENIX);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_PHOENIX);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_ROBOTICSFACILITY);
		bo.addItem(UnitId::PROTOSS_ZEALOT);
		bo.addItem(UnitId::PROTOSS_FLEETBEACON);
		bo.addItem(UnitId::PROTOSS_PYLON);
		bo.addItem(UnitId::PROTOSS_OBSERVER);
		bo.addItem(UnitId::PROTOSS_IMMORTAL);
		bo.addItem(UnitId::PROTOSS_CARRIER);
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