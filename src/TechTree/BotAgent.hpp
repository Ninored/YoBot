#pragma once

#include "sc2api/sc2_agent.h"

class BotAgent : public sc2::Agent {
  std::string version;
public:
  BotAgent();
  ~BotAgent();

  void OnGameStart();
  void OnStep();
};
