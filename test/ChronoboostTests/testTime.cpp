#define CATCH_CONFIG_MAIN
#include <iostream>
#include <vector>
#include "catch.hpp"
#include "BuildOrder.h"
#include "BOBuilder.h"
#include <fstream>

using namespace suboo;

TEST_CASE("Chronoboost reduces the time", "[chrono]") {
	GIVEN("An empty BuildOrder") {




		GameState gsi = TechTree::getTechTree().getInitial();
		GameState gsf();
		BuildOrder bo1(gsi);
		BuildOrder bo2(gsi);

		bo1.addItem(UnitId::PROTOSS_PYLON);
		bo1.addItem(UnitId::PROTOSS_GATEWAY);
		bo1.addItem(UnitId::PROTOSS_ZEALOT);

		bo2.addItem(UnitId::PROTOSS_PYLON);
		bo2.addItem(UnitId::PROTOSS_GATEWAY);
		bo2.addItem(UnitId::PROTOSS_ZEALOT);
		bo2.addItem(BuildItem(BuildAction::CHRONO, UnitId::PROTOSS_ZEALOT));

		timeBO(bo1);
		timeBO(bo2);

		int t1 = bo1.getFinal().getTimeStamp();
		int t2 = bo2.getFinal().getTimeStamp();

		REQUIRE(t2 < t1);
		









		/*

		BOBuilder builder;

		BuildGoal goal(0); // ASAP
		goal.addUnit(UnitId::PROTOSS_IMMORTAL, 1);
		goal.addUnit(UnitId::PROTOSS_PROBE, 10);

		builder.addGoal(goal);
		goal.print(std::cout);

		BuildOrder bo = builder.computeBO();
		bo.getInitial().addUnit(UnitId::PROTOSS_NEXUS);

		timeBO(bo);

		std::cout << "Initial realizable :" << std::endl;
		bo.print(std::cout);

		//int t1 = bo.getFinal.getTimeStamp();



		//version avec chronoboost
		BuildOrder bo2 = builder.computeBO();

		BuildItem bi(BuildAction::CHRONO);
		bi.setTarget(UnitId::PROTOSS_PROBE);
		bo2.addItem(bi);

		timeBO(bo2);

		std::cout << "Initial realizable :" << std::endl;
		bo.print(std::cout);

		//int t2 = bo2.getFinal.getTimeStamp();

		REQUIRE(t2 < t1);


		//int time = gsf.getTimeStamp();
		//std::cout << time << std::endl;

		//REQUIRE(bo == bo);
		//REQUIRE(std::equal(bo, bo));
		*/
	}
}
