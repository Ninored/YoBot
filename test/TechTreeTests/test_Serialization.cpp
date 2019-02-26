#define CATCH_CONFIG_MAIN
#include <iostream>
#include <vector>
#include "TechTree/TechTree.h"
#include "catch.hpp"
#include "nlohmann/json.hpp"

using namespace suboo;
using namespace nlohmann;

SCENARIO("TechTree can be serialized and deserialized", "[TechTree]") {
  GIVEN("A TechTree instance with no Units") {
    std::string empty = TechTree::getTechTree().serialize();
    std::string empty_result = json::parse("{\"version\": \"\"}").dump(2);

    REQUIRE(empty == empty_result);

    WHEN("adding a unit") {
      // Add unit into tech tree
      Unit u{
          1,                                                // id
          "test_name",                                      // name
          2,                                                // mineral_cost
          3,                                                // vespene_cost
          4,                                                // food_provided
          std::vector<AbilityId>{sc2::ABILITY_ID::ATTACK},  // abilities
          sc2::UNIT_TYPEID::PROTOSS_MOTHERSHIP,             // requirements
          0,                                                // production_time
          0,                                                // travel_time
          Unit::Status::TRAVELLING,                         // Status
      };
      TechTree::getTechTree().addUnit(u);

      json str = json::parse(
          "{\"units\":[{\"abilities\":[3674],\"food_provided\":4,\"action_"
          "status\":1,\"id\":1,\"mineral_cost\":2,\"name\":\"test_name\","
          "\"requirement\":10,\"vespene_cost\":3}],\"version\": \"\"}");
      std::string add_unit_true = str.dump(2);
      std::string add_unit_serialized = TechTree::getTechTree().serialize();

      THEN("the TechTree contains the unit added") {
        REQUIRE(add_unit_serialized == add_unit_true);
      }
    }
    
    WHEN("deserializing a string") {
      TechTree::getTechTree().clear();

      std::string serialized{
          "{\"units\":[{\"abilities\":[3674],\"food_provided\":4,\"action_"
          "status\":1,\"id\":1,\"mineral_cost\":2,\"name\":\"test_name\","
          "\"requirement\":10,\"vespene_cost\":3}],\"version\":\"\"}"};
      std::string truth = json::parse(serialized).dump(2);

      TechTree::getTechTree().deserialize("test_1.json");

      std::string result = TechTree::getTechTree().serialize();

      THEN("the TechTree remains the same") { REQUIRE(truth == result); }
    }
  }
}
