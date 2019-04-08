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
		BuildOrder bo1(gsi);
		BuildOrder bo2(gsi);

		bo1.addItem(UnitId::PROTOSS_PYLON);
		bo1.addItem(UnitId::PROTOSS_GATEWAY);
		bo1.addItem(UnitId::PROTOSS_ZEALOT);
		bo1.addItem(BuildAction::WAIT_GOAL);

		bo2.addItem(UnitId::PROTOSS_PYLON);
		bo2.addItem(UnitId::PROTOSS_GATEWAY);
		bo2.addItem(UnitId::PROTOSS_ZEALOT);
		bo2.addItem(BuildItem(BuildAction::CHRONO, UnitId::PROTOSS_ZEALOT));
		bo2.addItem(BuildAction::WAIT_GOAL);

		timeBO(bo1);
		timeBO(bo2);

		int t1 = bo1.getFinal().getTimeStamp();
		int t2 = bo2.getFinal().getTimeStamp();

		std::cout << "sans chrono : " << t1 << std::endl;
		std::cout << "avec chrono : " << t2 << std::endl;

		//le temps avec chronoboost est plus petit que sans
		REQUIRE(t2 < t1);

		//l'�nergie du nexus a bien �t� soustraite
		GameState gsf = bo2.getFinal();
		int e1;
		int e2;
		auto & fu = gsi.getFreeUnits();
		for (auto & ui : fu) {
			if (ui.type == UnitId::PROTOSS_NEXUS) {
				std::cout << "energy du nexus avant chronoboost :" << ui.energy << std::endl;
				e1 = ui.energy;
			}
		}
		fu = gsf.getFreeUnits();
		for (auto & ui : fu) {
			if (ui.type == UnitId::PROTOSS_NEXUS) {
				std::cout << "energy du nexus apr�s chronoboost :" << ui.energy << std::endl;
				e2 = ui.energy;
			}
		}

		REQUIRE(e2 == e1 - 50);
		

		







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
