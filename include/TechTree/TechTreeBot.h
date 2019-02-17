#ifndef YOBOT_SRC_TECHTREE_BOTAGENT_H
#define YOBOT_SRC_TECHTREE_BOTAGENT_H

#include "sc2api/sc2_agent.h"

namespace suboo {

class TechTreeBot : public sc2::Agent {
 public:
  TechTreeBot();
  ~TechTreeBot();

  virtual void OnGameStart() final;
  virtual void OnStep() final;
};

}  // namespace suboo
#endif  // YOBOT_SRC_TECHTREE_BOTAGENT_H
