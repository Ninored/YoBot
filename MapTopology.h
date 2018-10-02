#pragma once

#include <vector>
#include "sc2api/sc2_map_info.h"
#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_unit.h"


class MapTopology
{
	
	// map "topology" consists in tagging the expansions as being one of the following
	std::vector<int> mainBases;
	std::vector<int> pocketBases;
	std::vector<int> naturalBases;
	std::vector<int> proxyBases;
	// each base has a staging area, an open ground to group units before attacking
	std::vector<sc2::Point2D> stagingFor;	
	int ourBaseStartLocIndex;
	// Calculates expansion locations, this call can take on the order of 100ms since it makes blocking queries to SC2 so call it once and cache the reults.
	// it is modified from original provided by sc2API
	static std::vector<std::pair<sc2::Point3D, sc2::Units > > CalculateExpansionLocations(const sc2::ObservationInterface* observation, sc2::QueryInterface* query);
public:
	// raw cache for locations computed for expansions
	std::vector<sc2::Point3D> expansions;
	std::vector<sc2::Units> resourcesPer;
	// a main is a starting location
	// a nat is the place you would put your second CC normally
	// a proxy is an expo that is not immediately obvious to defender, but not too far
	// a pocket occurs when a base is reachable only by passing through your main
	enum BaseType {main,nat,proxy,pocket};
	enum Player {ally, enemy};
	// note asking for pocket will yield nat if no pocket found
	const sc2::Point3D & getPosition(Player p, BaseType b) const;
	int getExpansionIndex(Player p, BaseType b) const;
	// true iff we found pocket  bases
	bool hasPockets() const;
	// looking for a base ?
	const sc2::Point3D & FindNearestBase(const sc2::Point3D& start) const;
	// call this at game start to build up the info
	void init(const sc2::ObservationInterface * initial, sc2::QueryInterface * query, sc2::DebugInterface * debug=nullptr);
	// call this to see what the topology thinks in debug mode
	void debugMap(sc2::DebugInterface * debug);
};

namespace sc2util {
	// query pathing grid at given point
	bool Pathable(const sc2::GameInfo & info, const sc2::Point2D & point);
	// query build grid at given point
	bool Placement(const sc2::GameInfo & info, const sc2::Point2D & point);
	bool PlacementI(const sc2::GameInfo & info, const sc2::Point2DI & pointI);
}