#include "Placement.h"
#include "sc2api/sc2_interfaces.h"
#include "Pathing.h"
#include "ImageUtil.h"

using namespace std;
using namespace sc2;
using namespace sc2util;

void BuildingPlacer::debug(sc2::DebugInterface * debug, const sc2::ObservationInterface * obs, const GameInfo & info)
{
	// kinda costly, disabled by default
	if (true) {
		int res = 0;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (reserved[y*width + x]) {

					// auto z = expansions[FindNearestBaseIndex(Point2D(x, y))].z;
					auto z = obs->TerrainHeight(Point2D(x, y));
					auto len = to_string(z);
					len.erase(len.length() - 4);
					auto col = Colors::Red;
					if (Pathable(info, Point2D(x, y))) {
						col = Colors::Green;
					}
					//debug->DebugTextOut(len, Point3D(x + .5, y + .5, z + .3f), col);
					debug->DebugBoxOut(Point3D(x, y, z), Point3D(x + 1, y + 1, z + .3f), col);
					res++;
				}
			}
		}
		debug->DebugTextOut("Reserved :" + to_string(res));
	}
}


bool BuildingPlacer::Placement(const sc2::GameInfo & info, const sc2::Point2D & point) const
{
	return PlacementI(info, sc2::Point2DI((int)point.x, (int)point.y));
}


bool  BuildingPlacer::PlacementI(const sc2::GameInfo & info, const sc2::Point2DI & pointI) const
{
	if (pointI.x < 0 || pointI.x >= width || pointI.y < 0 || pointI.y >= height)
	{
		return false;
	}
	if (reserved[pointI.y*width + pointI.x]) {
		return false;
	}
	unsigned char encodedPlacement = info.placement_grid.data[pointI.x + ((height - 1) - pointI.y) * width];
	bool decodedPlacement = encodedPlacement == 255 ? true : false;
	return decodedPlacement;
}

bool BuildingPlacer::PlacementB(const sc2::GameInfo & info, const sc2::Point2D & point, int footprint) const
{
	auto inzone = sc2::Point2DI((int)point.x, (int)point.y);
	for (int x = 0; x < footprint; x++) {
		for (int y = 0; y < footprint; y++) {
			if (!PlacementI(info, Point2DI(inzone.x + x, inzone.y + y))) {
				return false;
			}
		}
	}
	return true;
}

bool BuildingPlacer::setBuildingAt(sc2::GameInfo & info, const sc2::Point2D & pos, int foot, bool b)
{
	return sc2util::setBuildingAt(info, info.placement_grid, pos, foot, b);
}

void BuildingPlacer::reserve(const sc2::Point2D & point)
{
	auto p = sc2::Point2DI((int)point.x, (int)point.y);
	reserved[p.y*width + p.x] = true;
}

void BuildingPlacer::reserveVector(const Point2D & start, const Point2D & vec) {
	auto vnorm = vec;
	auto len = Distance2D(vnorm, { 0,0 });
	vnorm /= len;
	for (float f = 2; f < len; f += .25) {
		auto torem = start + vnorm * f;
		reserve(torem);
		reserve(torem + Point2D(1, 0));
		reserve(torem + Point2D(0, 1));
		reserve(torem + Point2D(-1, 0));
		reserve(torem + Point2D(0, -1));
	}
}

void BuildingPlacer::reserve(int expIndex)
{
	for (auto & r : map->resourcesPer[expIndex]) {
		auto cc = map->expansions[expIndex];
		reserveVector(cc, (r->pos - cc));
	}
}

void BuildingPlacer::reserveCliffSensitive(int expIndex, const ObservationInterface * obs, const GameInfo & info)
{
	// scan tiles of the expansion (within 15.0), discard any that are within 5.0 of a cliff
	auto center = map->expansions[expIndex];
	int hx = center.x;
	int hy = center.y;
	auto h = obs->TerrainHeight(center);
	for (int i = -15; i <= 15; i++) {
		for (int j = -15; j <= 15; j++) {
			auto p = Point2D(hx + i, hy + j);
			if (obs->TerrainHeight(p) > h &&  Pathable(info, p)) {
				auto vec = center - p;
				auto vl = Distance2D(vec, Point2D(0, 0));
				vec /= vl;
				vec *= 5;
				reserveVector(p, vec);
			}
		}
	}
}



void BuildingPlacer::init(const sc2::ObservationInterface * initial, sc2::QueryInterface * query,  const MapTopology * map, sc2::DebugInterface * debug)	
{
	const GameInfo& game_info = initial->GetGameInfo();
	this->width = game_info.width;
	this->height = game_info.height;
	this->reserved = vector<int>(height*width, 0);
	this->map = map;
}
