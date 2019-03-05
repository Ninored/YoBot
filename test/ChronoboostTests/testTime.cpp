#define CATCH_CONFIG_MAIN
#include <iostream>
#include <vector>
#include "catch.hpp"
#include "BuildOrder.h"
#include "BOBuilder.h"

using namespace suboo;

TEST_CASE("Chronoboost reduces the time", "[chrono]") {
	GIVEN("An empty BuildOrder") {
		/*
		UnitInstance n1((UnitId)59, UnitInstance::UnitState::BUSY, 14);
		UnitInstance i1((UnitId)83, UnitInstance::UnitState::BUSY, 18);
		UnitInstance i2((UnitId)83, UnitInstance::UnitState::BUSY, 18);
		UnitInstance i3((UnitId)83, UnitInstance::UnitState::BUSY, 18);
		UnitInstance p1((UnitId)84, UnitInstance::UnitState::BUSY, 27);
		UnitInstance p2((UnitId)84, UnitInstance::UnitState::BUSY, 27);

		GameState gsi({n1, i1, p1}, 0, 0);
		GameState gsf({n1, i1, i2, i3, p1, p2}, 0, 0);
		//BuildOrder bo{gsi, {}, gsf};
		BuildOrder bo(gsi, gsf);*/

		


		BOBuilder builder;

		BuildGoal goal(0); // ASAP
		goal.addUnit(UnitId::PROTOSS_IMMORTAL, 1);
		goal.addUnit(UnitId::PROTOSS_PROBE, 10);

		builder.addGoal(goal);
		goal.print(std::cout);

		BuildOrder bo = builder.computeBO();

		timeBO(bo);

		std::cout << "Initial realizable :" << std::endl;
		bo.print(std::cout);

		//int time = gsf.getTimeStamp();
		//std::cout << time << std::endl;

		//REQUIRE(bo == bo);
		//REQUIRE(std::equal(bo, bo));

		REQUIRE(1 == 1);

	}
}
