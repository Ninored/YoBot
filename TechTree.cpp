// This file was generated by TechBot, do not edit. Run TechBot again to regenerate.
#include "BuildOrder.h"
namespace suboo {
TechTree::TechTree() :
	initial({ UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)59),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
 UnitInstance( (UnitId)84, UnitInstance::MINING_MINERALS, 0),
	}) {
  initial.getMinerals() = 50;
  initial.getVespene() = 0;
  units = {
{	0, // index
	(UnitId)4,  // ID
	"Colossus", // name
	300, // gold
	200, // gas
	-6,  // food
	(UnitId)71,  // builder unit  
	(UnitId)70, // tech requirement 
	53, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	1, // index
	(UnitId)10,  // ID
	"Mothership", // name
	400, // gold
	400, // gas
	-8,  // food
	(UnitId)59,  // builder unit  
	(UnitId)0, // tech requirement 
	114, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	2, // index
	(UnitId)59,  // ID
	"Nexus", // name
	400, // gold
	0, // gas
	15,  // food
	(UnitId)84,  // builder unit  
	(UnitId)0,  // tech requirement  
	71, // build time
	10,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	3, // index
	(UnitId)60,  // ID
	"Pylon", // name
	100, // gold
	0, // gas
	8,  // food
	(UnitId)84,  // builder unit  
	(UnitId)0,  // tech requirement  
	17, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	4, // index
	(UnitId)61,  // ID
	"Assimilator", // name
	75, // gold
	0, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)0,  // tech requirement  
	21, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	5, // index
	(UnitId)62,  // ID
	"Gateway", // name
	150, // gold
	0, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)60,  // tech requirement  
	46, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	6, // index
	(UnitId)63,  // ID
	"Forge", // name
	150, // gold
	0, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)59,  // tech requirement  
	32, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	7, // index
	(UnitId)64,  // ID
	"FleetBeacon", // name
	300, // gold
	200, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)67,  // tech requirement  
	42, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	8, // index
	(UnitId)65,  // ID
	"TwilightCouncil", // name
	150, // gold
	100, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)72,  // tech requirement  
	35, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	9, // index
	(UnitId)66,  // ID
	"PhotonCannon", // name
	150, // gold
	0, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)63,  // tech requirement  
	28, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	10, // index
	(UnitId)67,  // ID
	"Stargate", // name
	150, // gold
	150, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)72,  // tech requirement  
	42, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	11, // index
	(UnitId)68,  // ID
	"TemplarArchive", // name
	150, // gold
	200, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)65,  // tech requirement  
	35, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	12, // index
	(UnitId)69,  // ID
	"DarkShrine", // name
	150, // gold
	150, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)65,  // tech requirement  
	71, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	13, // index
	(UnitId)70,  // ID
	"RoboticsBay", // name
	150, // gold
	150, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)71,  // tech requirement  
	46, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	14, // index
	(UnitId)71,  // ID
	"RoboticsFacility", // name
	200, // gold
	100, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)72,  // tech requirement  
	46, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	15, // index
	(UnitId)72,  // ID
	"CyberneticsCore", // name
	150, // gold
	0, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)62,  // tech requirement  
	35, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	16, // index
	(UnitId)73,  // ID
	"Zealot", // name
	100, // gold
	0, // gas
	-2,  // food
	(UnitId)62,  // builder unit  
	(UnitId)0, // tech requirement 
	27, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	17, // index
	(UnitId)74,  // ID
	"Stalker", // name
	125, // gold
	50, // gas
	-2,  // food
	(UnitId)62,  // builder unit  
	(UnitId)72, // tech requirement 
	30, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	18, // index
	(UnitId)75,  // ID
	"HighTemplar", // name
	50, // gold
	150, // gas
	-2,  // food
	(UnitId)62,  // builder unit  
	(UnitId)68, // tech requirement 
	39, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	19, // index
	(UnitId)76,  // ID
	"DarkTemplar", // name
	125, // gold
	125, // gas
	-2,  // food
	(UnitId)62,  // builder unit  
	(UnitId)69, // tech requirement 
	39, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	20, // index
	(UnitId)77,  // ID
	"Sentry", // name
	50, // gold
	100, // gas
	-2,  // food
	(UnitId)62,  // builder unit  
	(UnitId)72, // tech requirement 
	26, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	21, // index
	(UnitId)78,  // ID
	"Phoenix", // name
	150, // gold
	100, // gas
	-2,  // food
	(UnitId)67,  // builder unit  
	(UnitId)0, // tech requirement 
	25, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	22, // index
	(UnitId)79,  // ID
	"Carrier", // name
	350, // gold
	250, // gas
	-6,  // food
	(UnitId)67,  // builder unit  
	(UnitId)64, // tech requirement 
	64, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	23, // index
	(UnitId)80,  // ID
	"VoidRay", // name
	250, // gold
	150, // gas
	-4,  // food
	(UnitId)67,  // builder unit  
	(UnitId)0, // tech requirement 
	42, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	24, // index
	(UnitId)81,  // ID
	"WarpPrism", // name
	200, // gold
	0, // gas
	-2,  // food
	(UnitId)71,  // builder unit  
	(UnitId)0, // tech requirement 
	35, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	25, // index
	(UnitId)82,  // ID
	"Observer", // name
	25, // gold
	75, // gas
	-1,  // food
	(UnitId)71,  // builder unit  
	(UnitId)0, // tech requirement 
	21, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	26, // index
	(UnitId)83,  // ID
	"Immortal", // name
	250, // gold
	100, // gas
	-4,  // food
	(UnitId)71,  // builder unit  
	(UnitId)0, // tech requirement 
	39, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	27, // index
	(UnitId)84,  // ID
	"Probe", // name
	50, // gold
	0, // gas
	-1,  // food
	(UnitId)59,  // builder unit  
	(UnitId)0, // tech requirement 
	12, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	28, // index
	(UnitId)133,  // ID
	"WarpGate", // name
	150, // gold
	0, // gas
	0,  // food
	(UnitId)62,  // builder unit  
	(UnitId)0,  // tech requirement  
	7, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	29, // index
	(UnitId)136,  // ID
	"WarpPrismPhasing", // name
	200, // gold
	0, // gas
	-2,  // food
	(UnitId)81,  // builder unit  
	(UnitId)0, // tech requirement 
	1, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	30, // index
	(UnitId)311,  // ID
	"Adept", // name
	100, // gold
	25, // gas
	-2,  // food
	(UnitId)62,  // builder unit  
	(UnitId)72, // tech requirement 
	27, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	31, // index
	(UnitId)495,  // ID
	"Oracle", // name
	150, // gold
	150, // gas
	-3,  // food
	(UnitId)67,  // builder unit  
	(UnitId)0, // tech requirement 
	37, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	32, // index
	(UnitId)496,  // ID
	"Tempest", // name
	250, // gold
	175, // gas
	-5,  // food
	(UnitId)67,  // builder unit  
	(UnitId)64, // tech requirement 
	42, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	33, // index
	(UnitId)694,  // ID
	"Disruptor", // name
	150, // gold
	150, // gas
	-3,  // food
	(UnitId)71,  // builder unit  
	(UnitId)70, // tech requirement 
	35, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	34, // index
	(UnitId)732,  // ID
	"OracleStasisTrap", // name
	0, // gold
	0, // gas
	0,  // food
	(UnitId)495,  // builder unit  
	(UnitId)0, // tech requirement 
	3, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
{	35, // index
	(UnitId)1910,  // ID
	"ShieldBattery", // name
	100, // gold
	0, // gas
	0,  // food
	(UnitId)84,  // builder unit  
	(UnitId)72,  // tech requirement  
	28, // build time
	4,   // travel time
	Unit::TRAVEL  // probe behavior
},
{	36, // index
	(UnitId)1911,  // ID
	"ObserverSiegeMode", // name
	25, // gold
	75, // gas
	-1,  // food
	(UnitId)82,  // builder unit  
	(UnitId)0, // tech requirement 
	0, // build time
	0,   // travel time
	Unit::BUSY  // producer behavior
},
  }; // end units
  indexes=std::vector<int>(1912,0); 
  indexes[4] = 0;
  indexes[10] = 1;
  indexes[59] = 2;
  indexes[60] = 3;
  indexes[61] = 4;
  indexes[62] = 5;
  indexes[63] = 6;
  indexes[64] = 7;
  indexes[65] = 8;
  indexes[66] = 9;
  indexes[67] = 10;
  indexes[68] = 11;
  indexes[69] = 12;
  indexes[70] = 13;
  indexes[71] = 14;
  indexes[72] = 15;
  indexes[73] = 16;
  indexes[74] = 17;
  indexes[75] = 18;
  indexes[76] = 19;
  indexes[77] = 20;
  indexes[78] = 21;
  indexes[79] = 22;
  indexes[80] = 23;
  indexes[81] = 24;
  indexes[82] = 25;
  indexes[83] = 26;
  indexes[84] = 27;
  indexes[133] = 28;
  indexes[136] = 29;
  indexes[311] = 30;
  indexes[495] = 31;
  indexes[496] = 32;
  indexes[694] = 33;
  indexes[732] = 34;
  indexes[894] = 35;
  indexes[1910] = 35;
  indexes[1911] = 36;
} //end ctor 
} //end ns 
