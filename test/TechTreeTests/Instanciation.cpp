#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TechTree/TechTree.hpp"

using namespace suboo;

TEST_CASE("Initialisation", "[TechTree]") {
  TechTree::getTechTree();
  REQUIRE( 1 == 1);
}