#include <fstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <nlohmann/json.hpp>
#include "TechTree/TechTree.h"
#include "TechTree/TechTreeBot.h"
#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"
#include "sc2utils/sc2_manage_process.h"

using namespace suboo;

int main(int argc, char **argv) {
  std::cout << "Starting TechTreeGenerator ..." << std::endl;
  sc2::Coordinator coordinator;

  if (!coordinator.LoadSettings(argc, argv)) {
    std::cout << "Cannot load settings." << std::endl;
    return -1;
  }

  auto path(coordinator.GetExePath());
  auto map = ".\\CeruleanFallLE.SC2Map";

  std::cout << "Path: " << path << std::endl;


  // Get version
  std::smatch matched;
  std::regex reg("[0-9]{5}");
  std::string version = "00000";

  if (std::regex_search(path, matched, reg)) {
    version = matched[matched.size() - 1];
  }
  std::cout << "Version :" << version << std::endl;
  TechTree::getTechTree().setVersion(version);

  TechTreeBot bot;
  coordinator.SetStepSize(1);
  coordinator.SetRealtime(false);
  coordinator.SetMultithreaded(true);

  coordinator.SetParticipants(
      {CreateParticipant(sc2::Race::Protoss, &bot),
       CreateComputer(sc2::Race::Protoss, sc2::Difficulty::CheatInsane)});
  coordinator.LaunchStarcraft();  // Launch game

  if (coordinator.StartGame(map)) {
    for (int i = 0; i < 10; ++i) coordinator.Update();
    coordinator.LeaveGame();
  } else {
    std::cout << "Error while loading the map: " << map << std::endl;
  }

  std::cout << "Writing json file..." << std::endl;
  std::ofstream file;
  file.open("./techtree_" + version + ".json", std::ios::out);
  if (file.is_open()) {
    file << TechTree::getTechTree().serialize();
    file.close();
  }

  std::cout << "TechTreeGenerator closing..." << std::endl;
  return 0;
}
